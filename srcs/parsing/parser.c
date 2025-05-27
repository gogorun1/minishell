#include "minishell.h"

ast_node_t *parse_command(parser_t *parser) {
    if (!parser->current || parser->current->type == TOKEN_EOF || 
        parser->current->type == TOKEN_PIPE)
        return NULL;
    
    // Collect arguments until we hit a pipe or EOF
    char **args = NULL;
    int arg_count = 0;
    
    printf("    Parsing command starting with: '%s'\n", 
           parser->current->value ? parser->current->value : "NULL");
    
    while (parser->current && parser->current->type == TOKEN_WORD) {
        args = realloc(args, sizeof(char*) * (arg_count + 2));
        args[arg_count] = strdup(parser->current->value);
        args[arg_count + 1] = NULL;
        printf("      Added arg[%d]: '%s'\n", arg_count, args[arg_count]);
        arg_count++;
        parser->current = parser->current->next;
    }
    
    if (arg_count == 0) return NULL;
    
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_COMMAND;
    node->data.command.args = args;
    node->data.command.redirs = NULL;
    
    printf("    Created command node with %d args\n", arg_count);
    return node;
}

ast_node_t *parse_pipeline(parser_t *parser) {
    printf("  Starting pipeline parsing\n");
    
    // Parse first command
    ast_node_t *left = parse_command(parser);
    if (!left) {
        printf("  No left command found\n");
        return NULL;
    }
    
    printf("  Got left command, current token: %s\n", 
           parser->current && parser->current->value ? parser->current->value : "NULL/EOF");
    
    // Keep parsing pipes
    while (parser->current && parser->current->type == TOKEN_PIPE) {
        printf("  Found pipe! Continuing pipeline...\n");
        parser->current = parser->current->next; // Skip the pipe token
        
        ast_node_t *right = parse_command(parser);
        if (!right) {
            printf("  ERROR: No command after pipe\n");
            // free_ast(left); // Commented out for demo
            return NULL;
        }
        
        printf("  Got right command, creating pipe node\n");
        
        // Create pipe node with left and right
        ast_node_t *pipe_node = malloc(sizeof(ast_node_t));
        pipe_node->type = AST_PIPE;
        pipe_node->data.binary.left = left;
        pipe_node->data.binary.right = right;
        
        // The pipe node becomes the new left for the next iteration
        left = pipe_node;
        
        printf("  Pipe node created, checking for more pipes...\n");
    }
    
    printf("  Pipeline parsing complete\n");
    return left;
}

ast_node_t *parse(t_token *tokens) {
    parser_t parser;
    parser.tokens = tokens;
    parser.current = tokens;
    
    return parse_pipeline(&parser);
}