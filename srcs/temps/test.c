#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Placeholder Definitions (You should have these in your actual project) ---
// These are minimal definitions to make the provided code compile.
// Replace them with your actual, complete definitions.

// Token types (add new ones for redirections)
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_LT,       // <
    TOKEN_GT,       // >
    TOKEN_GTGT,     // >>
    TOKEN_LTLT,     // << (Here-document, often requires more complex parsing)
    TOKEN_EOF
} t_token_type;

// Token structure
typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

// Redirection types
typedef enum e_redir_type {
    REDIR_IN,       // <
    REDIR_OUT,      // >
    REDIR_APPEND,   // >>
    REDIR_HEREDOC   // <<
} t_redir_type;

// Structure for a single redirection
typedef struct s_redir {
    t_redir_type type;
    char *filename;
    struct s_redir *next; // Linked list for multiple redirections per command
} t_redir;

// AST Command Node Data
typedef struct s_ast_command {
    char **args;
    t_redir *redirs; // Head of the linked list of redirections
} t_ast_command;

// AST Node types
typedef enum e_ast_node_type {
    AST_COMMAND,
    AST_PIPE
} t_ast_node_type;

// AST Node structure
typedef struct s_ast_node {
    t_ast_node_type type;
    union {
        t_ast_command command;
        struct {
            struct s_ast_node *left;
            struct s_ast_node *right;
        } binary; // For pipe nodes
    } data;
} ast_node_t;

// Parser structure
typedef struct s_parser {
    t_token *tokens;
    t_token *current;
} parser_t;

// --- End of Placeholder Definitions ---


// Helper function to free a linked list of redirections
void free_redirections(t_redir *redirs) {
    t_redir *current = redirs;
    t_redir *next;
    while (current) {
        next = current->next;
        free(current->filename); // Free the filename string
        free(current);           // Free the redirection struct
        current = next;
    }
}

// Helper function to free an AST node (simplified for this example)
// In a real shell, this would recursively free child nodes.
void free_ast(ast_node_t *node) {
    if (!node) return;

    if (node->type == AST_COMMAND) {
        if (node->data.command.args) {
            for (int i = 0; node->data.command.args[i]; i++) {
                free(node->data.command.args[i]);
            }
            free(node->data.command.args);
        }
        free_redirections(node->data.command.redirs);
    } else if (node->type == AST_PIPE) {
        free_ast(node->data.binary.left);
        free_ast(node->data.binary.right);
    }
    free(node);
}

// Helper function to add a redirection to the command's list
void add_redirection(t_ast_command *command_data, t_redir_type type, char *filename) {
    t_redir *new_redir = malloc(sizeof(t_redir));
    if (!new_redir) {
        perror("malloc error for redirection");
        exit(EXIT_FAILURE); // Or handle error more gracefully
    }
    new_redir->type = type;
    new_redir->filename = strdup(filename); // Duplicate filename to own the memory
    new_redir->next = NULL;

    if (!command_data->redirs) {
        command_data->redirs = new_redir;
    } else {
        t_redir *current = command_data->redirs;
        while (current->next) {
            current = current->next;
        }
        current->next = new_redir;
    }
    printf("        Added redirection: type %d, file '%s'\n", type, filename);
}

// Parses a single command, including its arguments and redirections.
// A command can have arguments and redirections intermixed (e.g., "cmd <in arg1 >out arg2").
ast_node_t *parse_command(parser_t *parser) {
    // If current token is EOF or PIPE, it's not the start of a command
    if (!parser->current || parser->current->type == TOKEN_EOF ||
        parser->current->type == TOKEN_PIPE) {
        printf("    parse_command: No command or pipe/EOF encountered, returning NULL.\n");
        return NULL;
    }

    char **args = NULL;
    int arg_count = 0;
    ast_node_t *node = NULL; // Initialize node to NULL

    printf("    Parsing command/redirections starting with: '%s' (type: %d)\n",
           parser->current->value ? parser->current->value : "NULL", parser->current->type);

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
            printf("      Added arg[%d]: '%s'\n", arg_count, args[arg_count]);
            arg_count++;
            parser->current = parser->current->next; // Consume the word token
        } else if (parser->current->type == TOKEN_LT) { // Handle '<' (input redirection)
            parser->current = parser->current->next; // Consume '<'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_IN, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '<'\n");
                free_ast(node); // Clean up partially created node
                return NULL; // Syntax error
            }
        } else if (parser->current->type == TOKEN_GT) { // Handle '>' (output redirection)
            parser->current = parser->current->next; // Consume '>'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_OUT, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '>'\n");
                free_ast(node);
                return NULL;
            }
        } else if (parser->current->type == TOKEN_GTGT) { // Handle '>>' (append output redirection)
            parser->current = parser->current->next; // Consume '>>'
            if (parser->current && parser->current->type == TOKEN_WORD) {
                add_redirection(&(node->data.command), REDIR_APPEND, parser->current->value);
                parser->current = parser->current->next; // Consume filename
            } else {
                fprintf(stderr, "Parse error: Expected filename after '>>'\n");
                free_ast(node);
                return NULL;
            }
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
        printf("    parse_command: No arguments or redirections found, returning NULL.\n");
        free_ast(node); // Free the allocated node as it's empty
        return NULL;
    }

    node->data.command.args = args; // Assign the collected arguments
    // node->data.command.redirs is already populated by add_redirection

    printf("    Created command node with %d args and %s redirections\n",
           arg_count, node->data.command.redirs ? "some" : "no");
    return node;
}

// Parses a pipeline of commands (e.g., "cmd1 | cmd2 | cmd3").
ast_node_t *parse_pipeline(parser_t *parser) {
    printf("  Starting pipeline parsing\n");

    // Parse the first command in the pipeline
    ast_node_t *left = parse_command(parser);
    if (!left) {
        printf("  No left command found, pipeline parsing failed.\n");
        return NULL;
    }

    printf("  Got left command, current token: %s (type: %d)\n",
           parser->current && parser->current->value ? parser->current->value : "NULL/EOF",
           parser->current ? parser->current->type : TOKEN_EOF);

    // Keep parsing pipes and subsequent commands
    while (parser->current && parser->current->type == TOKEN_PIPE) {
        printf("  Found pipe! Continuing pipeline...\n");
        parser->current = parser->current->next; // Skip the pipe token

        // Parse the command on the right side of the pipe
        ast_node_t *right = parse_command(parser);
        if (!right) {
            fprintf(stderr, "  ERROR: No command found after pipe.\n");
            free_ast(left); // Free the partially built pipeline
            return NULL;
        }

        printf("  Got right command, creating pipe node\n");

        // Create a new pipe node, making the previous 'left' node its left child
        // and the newly parsed 'right' node its right child.
        ast_node_t *pipe_node = malloc(sizeof(ast_node_t));
        if (!pipe_node) {
            perror("malloc error for pipe node");
            free_ast(left);
            free_ast(right);
            return NULL;
        }
        pipe_node->type = AST_PIPE;
        pipe_node->data.binary.left = left;
        pipe_node->data.binary.right = right;

        // The new pipe node becomes the 'left' for the next iteration,
        // allowing for chained pipes (e.g., A | B | C becomes (A | B) | C).
        left = pipe_node;

        printf("  Pipe node created, checking for more pipes...\n");
    }

    printf("  Pipeline parsing complete.\n");
    return left; // Return the root of the pipeline AST
}

// Main parsing function
ast_node_t *parse(t_token *tokens) {
    parser_t parser;
    parser.tokens = tokens;
    parser.current = tokens; // Start parsing from the first token

    // A pipeline is the top-level construct we are parsing
    return parse_pipeline(&parser);
}

// Example usage (requires a lexer to generate tokens)
/*
// Mock lexer for demonstration
t_token *create_token(t_token_type type, const char *value) {
    t_token *token = malloc(sizeof(t_token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->next = NULL;
    return token;
}

t_token *mock_lexer(const char *input) {
    t_token *head = NULL;
    t_token *current = NULL;

    // Example: "ls -l > output.txt | grep foo < input.txt"
    if (strcmp(input, "ls -l > output.txt | grep foo < input.txt") == 0) {
        head = create_token(TOKEN_WORD, "ls");
        current = head;
        current->next = create_token(TOKEN_WORD, "-l");
        current = current->next;
        current->next = create_token(TOKEN_GT, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "output.txt");
        current = current->next;
        current->next = create_token(TOKEN_PIPE, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "grep");
        current = current->next;
        current->next = create_token(TOKEN_WORD, "foo");
        current = current->next;
        current->next = create_token(TOKEN_LT, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "input.txt");
        current = current->next;
        current->next = create_token(TOKEN_EOF, NULL);
    } else if (strcmp(input, "echo hello >> log.txt") == 0) {
        head = create_token(TOKEN_WORD, "echo");
        current = head;
        current->next = create_token(TOKEN_WORD, "hello");
        current = current->next;
        current->next = create_token(TOKEN_GTGT, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "log.txt");
        current = current->next;
        current->next = create_token(TOKEN_EOF, NULL);
    } else if (strcmp(input, "cat < file.txt") == 0) {
        head = create_token(TOKEN_WORD, "cat");
        current = head;
        current->next = create_token(TOKEN_LT, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "file.txt");
        current = current->next;
        current->next = create_token(TOKEN_EOF, NULL);
    } else if (strcmp(input, "just_a_command") == 0) {
        head = create_token(TOKEN_WORD, "just_a_command");
        current = head;
        current->next = create_token(TOKEN_EOF, NULL);
    } else if (strcmp(input, "command1 | command2") == 0) {
        head = create_token(TOKEN_WORD, "command1");
        current = head;
        current->next = create_token(TOKEN_PIPE, NULL);
        current = current->next;
        current->next = create_token(TOKEN_WORD, "command2");
        current = current->next;
        current->next = create_token(TOKEN_EOF, NULL);
    }
    // Add more mock cases as needed

    return head;
}

// Function to print the AST (for verification)
void print_ast(ast_node_t *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");
    if (node->type == AST_COMMAND) {
        printf("COMMAND:\n");
        for (int i = 0; i < indent + 1; i++) printf("  ");
        printf("Args: [");
        if (node->data.command.args) {
            for (int i = 0; node->data.command.args[i]; i++) {
                printf("'%s'%s", node->data.command.args[i],
                       node->data.command.args[i+1] ? ", " : "");
            }
        }
        printf("]\n");
        if (node->data.command.redirs) {
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Redirections:\n");
            t_redir *r = node->data.command.redirs;
            while (r) {
                for (int i = 0; i < indent + 2; i++) printf("  ");
                printf("Type: %d, File: '%s'\n", r->type, r->filename);
                r = r->next;
            }
        }
    } else if (node->type == AST_PIPE) {
        printf("PIPE:\n");
        print_ast(node->data.binary.left, indent + 1);
        print_ast(node->data.binary.right, indent + 1);
    }
}

// Function to free mock tokens
void free_mock_tokens(t_token *tokens) {
    t_token *current = tokens;
    t_token *next;
    while (current) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

int main() {
    printf("--- Test Case 1: ls -l > output.txt | grep foo < input.txt ---\n");
    t_token *tokens1 = mock_lexer("ls -l > output.txt | grep foo < input.txt");
    ast_node_t *ast1 = parse(tokens1);
    if (ast1) {
        printf("\nParsed AST:\n");
        print_ast(ast1, 0);
        free_ast(ast1);
    } else {
        printf("\nParsing failed for test case 1.\n");
    }
    free_mock_tokens(tokens1);
    printf("\n\n");

    printf("--- Test Case 2: echo hello >> log.txt ---\n");
    t_token *tokens2 = mock_lexer("echo hello >> log.txt");
    ast_node_t *ast2 = parse(tokens2);
    if (ast2) {
        printf("\nParsed AST:\n");
        print_ast(ast2, 0);
        free_ast(ast2);
    } else {
        printf("\nParsing failed for test case 2.\n");
    }
    free_mock_tokens(tokens2);
    printf("\n\n");

    printf("--- Test Case 3: cat < file.txt ---\n");
    t_token *tokens3 = mock_lexer("cat < file.txt");
    ast_node_t *ast3 = parse(tokens3);
    if (ast3) {
        printf("\nParsed AST:\n");
        print_ast(ast3, 0);
        free_ast(ast3);
    } else {
        printf("\nParsing failed for test case 3.\n");
    }
    free_mock_tokens(tokens3);
    printf("\n\n");

    printf("--- Test Case 4: just_a_command ---\n");
    t_token *tokens4 = mock_lexer("just_a_command");
    ast_node_t *ast4 = parse(tokens4);
    if (ast4) {
        printf("\nParsed AST:\n");
        print_ast(ast4, 0);
        free_ast(ast4);
    } else {
        printf("\nParsing failed for test case 4.\n");
    }
    free_mock_tokens(tokens4);
    printf("\n\n");

    printf("--- Test Case 5: command1 | command2 ---\n");
    t_token *tokens5 = mock_lexer("command1 | command2");
    ast_node_t *ast5 = parse(tokens5);
    if (ast5) {
        printf("\nParsed AST:\n");
        print_ast(ast5, 0);
        free_ast(ast5);
    } else {
        printf("\nParsing failed for test case 5.\n");
    }
    free_mock_tokens(tokens5);
    printf("\n\n");

    return 0;
}
*/
