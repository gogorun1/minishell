#include "minishell.h"

// Helper function to add a redirection to the command's list
void add_redirection(command_t *command_data, redir_type_t type, char *filename) {
    redir_t *new_redir = malloc(sizeof(redir_t));
    if (!new_redir) {
        perror("malloc error for redirection");
        exit(EXIT_FAILURE); // Or handle error more gracefully
    }
    new_redir->type = type;
    new_redir->file = strdup(filename); // Duplicate filename to own the memory
    new_redir->next = NULL;

    if (!command_data->redirs) {
        command_data->redirs = new_redir;
    } else {
        redir_t *current = command_data->redirs;
        while (current->next) {
            current = current->next;
        }
        current->next = new_redir;
    }
    // printf("        Added redirection: type %d, file '%s'\n", type, filename);
}

ast_node_t *parse_command(parser_t *parser) {
    // If current token is EOF or PIPE, it's not the start of a command
    if (!parser->current || parser->current->type == TOKEN_EOF ||
        parser->current->type == TOKEN_PIPE) {
        // printf("    parse_command: No command or pipe/EOF encountered, returning NULL.\n");
        return NULL;
    }

    char **args = NULL;
    int arg_count = 0;
    ast_node_t *node = NULL; // Initialize node to NULL

    // printf("    Parsing command/redirections starting with: '%s' (type: %d)\n",
        //    parser->current->value ? parser->current->value : "NULL", parser->current->type);

    // Allocate the command node early
    node = malloc(sizeof(ast_node_t));
    if (!node) {
        perror("malloc error for AST node");
        return NULL;
    }
    node->type = AST_COMMAND;
    node->data.command.args = NULL; // Initialize to NULL
    node->data.command.redirs = NULL; // Initialize to NULL

    // Loop to parse arguments and redirections until a pipe, EOF, or unexpected token
    while (parser->current &&
           parser->current->type != TOKEN_EOF &&
           parser->current->type != TOKEN_PIPE) {

        if (parser->current->type == TOKEN_WORD) {
            // It's an argument: add to args list
            args = realloc(args, sizeof(char*) * (arg_count + 2));
            if (!args) {
                perror("realloc error for args");
                free_ast(node); // Clean up partially created node
                return NULL;
            }
            args[arg_count] = strdup(parser->current->value);
            args[arg_count + 1] = NULL; // Null-terminate the argument list
            // printf("      Added arg[%d]: '%s'\n", arg_count, args[arg_count]);
            arg_count++;
            parser->current = parser->current->next; // Consume the word token
        } else if (parser->current->type == TOKEN_REDIRECT_IN) { // Handle '<' (input redirection)
            parser->current = parser->current->next; // Consume '<'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_IN, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '<'\n");
                free_ast(node); // Clean up partially created node
                return NULL; // Syntax error
            }
        } else if (parser->current->type == TOKEN_REDIRECT_OUT) { // Handle '>' (output redirection)
            parser->current = parser->current->next; // Consume '>'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_OUT, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '>'\n");
                free_ast(node);
                return NULL;
            }
        } else if (parser->current->type == TOKEN_APPEND) { // Handle '>>' (append output redirection)
            parser->current = parser->current->next; // Consume '>>'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_APPEND, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '>>'\n");
                free_ast(node);
                return NULL;
            }
		} else if (parser->current->type == TOKEN_HEREDOC) { // Handle '<<' (heredoc)
            parser->current = parser->current->next; // Consume '<<'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                // 使用 filename 字段存储 delimiter
                add_redirection(&(node->data.command), REDIR_HEREDOC, parser->current->value);
                parser->current = parser->current->next; // Consume delimiter
            } else {
                fprintf(stderr, "Parse error: Expected filename after '<<'\n");
                free_ast(node);
                return NULL;
            }
// ...
        // Add more cases for other redirection types like TOKEN_LTLT (heredoc) if needed
        // } else if (parser->current->type == TOKEN_LTLT) { ... }
        } else {
            // Unexpected token for a command part (neither word nor redirection)
            fprintf(stderr, "Parse error: Unexpected token '%s' (type: %d) in command\n",
                    parser->current->value ? parser->current->value : "NULL", parser->current->type);
            free_ast(node); // Clean up partially created node
            return NULL;
        }
    }

    // A command must have at least one argument OR at least one redirection.
    // If neither is present, it's not a valid command.
    if (arg_count == 0 && !node->data.command.redirs) {
        // printf("    parse_command: No arguments or redirections found, returning NULL.\n");
        free_ast(node); // Free the allocated node as it's empty
        return NULL;
    }

    node->data.command.args = args; // Assign the collected arguments
    // node->data.command.redirs is already populated by add_redirection

    // printf("    Created command node with %d args and %s redirections\n",
        //    arg_count, node->data.command.redirs ? "some" : "no");
    return node;
}

ast_node_t *parse_pipeline(parser_t *parser) {
    printf("  Starting pipeline parsing\n");
    
    // Parse first command
    ast_node_t *left = parse_command(parser);
    if (!left) {
        // printf("  No left command found\n");
        return NULL;
    }
    
    // printf("  Got left command, current token: %s\n", 
        //    parser->current && parser->current->value ? parser->current->value : "NULL/EOF");
    
    // Keep parsing pipes
    while (parser->current && parser->current->type == TOKEN_PIPE) {
        printf("  Found pipe! Continuing pipeline...\n");
        parser->current = parser->current->next; // Skip the pipe token
        
        ast_node_t *right = parse_command(parser);
        if (!right) {
            // printf("  ERROR: No command after pipe\n");
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
        
        // printf("  Pipe node created, checking for more pipes...\n");
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