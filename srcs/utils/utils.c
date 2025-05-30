#include "minishell.h"

// 检查字符是否可用于变量名
bool is_valid_var_char(char c)
{
    return (c == '_' || isalnum(c));
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

// 处理特殊字符
void handle_special_char(char *line, int *i, t_token **tokens)
{
    if (line[*i] == '|')
    {
        add_token(tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
        (*i)++;
    }
    else if (line[*i] == '<')
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
    else if (line[*i] == '>')
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
}

// 查找可执行文件
char *find_executable(char *cmd)
{
    // 如果命令包含路径分隔符，直接检查是否可执行
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    
    // 否则在PATH中查找
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

// char *expand_variable(char *str, t_shell g_shell)
// {
// 	char *result = NULL;
// 	char *temp;
// 	int i = 0;
// 	int start = 0;

// 	while (str[i])
// 	{
// 		if (str[i] == '$' && (i == 0 || str[i - 1] != '\\'))
// 		{
// 			// 添加前面的部分
// 			if (i > start)
// 			{
// 				temp = ft_strndup(str + start, i - start);
// 				result = ft_strjoin(result, temp);
// 				free(temp);
// 			}
// 			start = i + 1; // 跳过$符号

// 			// 查找变量名
// 			while (str[i] && is_valid_var_char(str[i]))
// 				i++;

// 			char *var_name = ft_strndup(str + start, i - start);
// 			char *var_value = my_getenv(var_name, g_shell.env_list); // 使用自定义getenv
// 			free(var_name);

// 			// 添加变量值
// 			if (var_value)
// 			{
// 				temp = ft_strjoin(result, var_value);
// 				free(result);
// 				result = temp;
// 				free(var_value);
// 			}
// 			start = i; // 更新起始位置
// 		}
// 		else
// 			i++;
// 	}

// 	// 添加剩余部分
// 	if (str[start])
// 	{
// 		temp = ft_strjoin(result, str + start);
// 		free(result);
// 		result = temp;
// 	}

// 	return result;
// }

void free_ast(ast_node_t *node) {
    if (!node)
        return;
    
    switch (node->type) {
        case AST_COMMAND:
            if (node->data.command.args) {
                for (int i = 0; node->data.command.args[i]; i++)
                    free(node->data.command.args[i]);
                free(node->data.command.args);
            }
            // Free redirections
            redir_t *redir = node->data.command.redirs;
            while (redir) {
                redir_t *next = redir->next;
                free(redir->file);
                free(redir);
                redir = next;
            }
            break;
		// case AST_REDIR:
        case AST_PIPE:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        // case AST_SUBSHELL:
        //     free_ast(node->data.child);
            break;
    }
    free(node);
}