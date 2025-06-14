#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (c == '_' || ft_isalnum(c));
}

// 检查是否为特殊字符
bool is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char *result;
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	size_t len3 = strlen(s3);

	result = (char *)malloc(len1 + len2 + len3 + 1);
	if (!result)
		return NULL;

	strcpy(result, s1);
	strcat(result, s2);
	strcat(result, s3);

	return result;
}

static void	handle_special_char_pipe(char *line, int *i, t_token **tokens)
{
	(void)line; // Mark 'line' as unused to avoid compiler warning
	add_token(tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
	(*i)++;
}

static void	handle_special_char_lt(char *line, int *i, t_token **tokens)
{
	if (line[*i + 1] == '<')
	{
		add_token(tokens, create_token(ft_strdup("<<"), TOKEN_HEREDOC));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, create_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
		(*i)++;
	}
}

static void	handle_special_char_gt(char *line, int *i, t_token **tokens)
{
	if (line[*i + 1] == '>')
	{
		add_token(tokens, create_token(ft_strdup(">>"), TOKEN_APPEND));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
		(*i)++;
	}
}

void	handle_special_char(char *line, int *i, t_token **tokens)
{
	if (line[*i] == '|')
		handle_special_char_pipe(line, i, tokens);
	else if (line[*i] == '<')
		handle_special_char_lt(line, i, tokens);
	else if (line[*i] == '>')
		handle_special_char_gt(line, i, tokens);
}

// 查找可执行文件
char *find_executable(char *cmd)
{
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    char *path_env = getenv("PATH");
    if (!path_env)
        return NULL;
        
    char *path_copy = ft_strdup(path_env);
    char *path_token = strtok(path_copy, ":");
    
    while (path_token)
    {
        char *full_path = ft_strjoin3(path_token, "/", cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        path_token = strtok(NULL, ":");
    }
    
    free(path_copy);
    return NULL;
}

// Custom getenv to work with your envp array
char *my_getenv(const char *key, t_env *envp) 
{
	// printf("my_getenv called with key: %s\n", key); // Debugging line
    if (!key || !envp) 
	{
        return NULL;
    }
	// printf("envp starting with: %s\n", envp->key); // Debugging line
	t_env *current = envp;
	// printf("Starting from: %s\n", current->key); // Debugging line
	while (current) 
	{
		// printf("Checking env key: %s, key: %s\n", current->key, key); // Debugging line
		if (strcmp(current->key, key) == 0) 
		{
			return current->value; // 返回环境变量的值
		}
		current = current->next;
	}
	return NULL; // 如果没有找到，返回NULL
}

// 获取环境变量值
char *get_env_value(char *var_name)
{
	char *value = getenv(var_name);
	if (value)
		return ft_strdup(value);
	return NULL;
}

void free_ast(ast_node_t *node) 
{
    if (!node)
        return;
    switch (node->type) {
        case AST_COMMAND:
            if (node->data.command.args) {
                for (int i = 0; node->data.command.args[i]; i++)
                    free(node->data.command.args[i]);
                free(node->data.command.args);
            }
            redir_t *redir = node->data.command.redirs;
            while (redir) {
                redir_t *next = redir->next;
                free(redir->file);
                free(redir);
                redir = next;
            }
            break;
        case AST_PIPE:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
            break;
    }
    free(node);
}
