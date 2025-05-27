#include "minishell.h"

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

const char* redir_type_to_string(redir_type_t type) {
    switch (type) {
        case REDIR_IN:     return "< (REDIR_IN)";
        case REDIR_OUT:    return "> (REDIR_OUT)";
        case REDIR_APPEND: return ">> (REDIR_APPEND)";
        case REDIR_HEREDOC:return "<< (REDIR_HEREDOC)";
        default:           return "Unknown Redirection Type";
    }
}

/**
 * @brief 递归打印抽象语法树 (AST) 的结构。
 *
 * @param node 要打印的当前 AST 节点。
 * @param indent_level 当前节点的缩进级别，用于可视化树的深度。
 */
void print_ast(ast_node_t *node, int indent_level)
{
    if (node == NULL) {
        return;
    }

    // 打印当前节点的缩进
    for (int i = 0; i < indent_level; ++i) {
        printf("  ");
    }

    if (node->type == AST_COMMAND) {
        printf("COMMAND:\n");

        // 打印命令的参数
        for (int i = 0; i < indent_level + 1; ++i) printf("  ");
        printf("Args: ");
        if (node->data.command.args && node->data.command.args[0]) {
            for (int i = 0; node->data.command.args[i] != NULL; ++i) {
                printf("'%s'%s", node->data.command.args[i], node->data.command.args[i+1] ? ", " : "");
            }
            printf("\n");
        } else {
            printf("[None]\n"); // 如果没有参数
        }

        // 打印命令的重定向
        if (node->data.command.redirs) {
            for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            printf("Redirections:\n");
            redir_t *current_redir = node->data.command.redirs;
            while (current_redir) {
                for (int i = 0; i < indent_level + 2; ++i) printf("  ");
                printf("Type: %s, File/Delimiter: '%s'\n",
                       redir_type_to_string(current_redir->type),
                       current_redir->file ? current_redir->file : "NULL");
                current_redir = current_redir->next;
            }
        } else {
            // 可选：如果没有任何重定向，可以明确打印出来
            // for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            // printf("Redirections: [None]\n");
        }

    } else if (node->type == AST_PIPE) {
        printf("PIPE:\n");

        // 递归打印管道的左侧命令/管道
        for (int i = 0; i < indent_level + 1; ++i) printf("  ");
        printf("Left:\n");
        print_ast(node->data.binary.left, indent_level + 2);

        // 递归打印管道的右侧命令/管道
        for (int i = 0; i < indent_level + 1; ++i) printf("  ");
        printf("Right:\n");
        print_ast(node->data.binary.right, indent_level + 2);

    } else {
        // 处理未知或未定义的 AST 节点类型
        for (int i = 0; i < indent_level; ++i) printf("  ");
        printf("Unknown AST Node Type\n");
    }
}
