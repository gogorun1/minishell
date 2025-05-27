#include "minishell.h"

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void print_ast_detailed(ast_node_t *node, int depth, const char *position) 
{
    if (!node) return;
    
    // Print indentation and position
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s", position);
    
    switch (node->type) {
        case AST_COMMAND:
            printf("COMMAND: ");
            if (node->data.command.args) {
                for (int i = 0; node->data.command.args[i]; i++) {
                    printf("'%s' ", node->data.command.args[i]);
                }
            }
            printf("\n");
            break;
        case AST_PIPE:
            printf("PIPE\n");
            print_ast_detailed(node->data.binary.left, depth + 1, "├─ left: ");
            print_ast_detailed(node->data.binary.right, depth + 1, "└─ right: ");
            break;
        // case AST_AND:
        //     printf("AND\n");
        //     print_ast_detailed(node->data.binary.left, depth + 1, "├─ left: ");
        //     print_ast_detailed(node->data.binary.right, depth + 1, "└─ right: ");
        //     break;
        // case AST_OR:
        //     printf("OR\n");
        //     print_ast_detailed(node->data.binary.left, depth + 1, "├─ left: ");
        //     print_ast_detailed(node->data.binary.right, depth + 1, "└─ right: ");
        //     break;
        default:
            printf("UNKNOWN\n");
    }
}