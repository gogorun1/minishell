#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void free_str_array(char **arr)
{
    int i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int	is_valid_var_char(char c)
{
	return (c == '_' || ft_isalnum(c));
}

// 检查是否为特殊字符
bool is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

char *ft_strcat(char *dest, const char *src)
{
	char *ptr = dest + ft_strlen(dest);
	while (*src)
	{
		*ptr++ = *src++;
	}
	*ptr = '\0';
	return dest;
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char *result;
	size_t len1 = ft_strlen(s1);
	size_t len2 = ft_strlen(s2);
	size_t len3 = ft_strlen(s3);

	result = (char *)malloc(len1 + len2 + len3 + 1);
	if (!result)
		return NULL;

	ft_strcpy(result, s1);
	ft_strcat(result, s2);
	ft_strcat(result, s3);

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
// Helper to handle the case where cmd contains a '/'
static char *handle_direct_path(char *cmd)
{
    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));
    return (NULL);
}

// Helper to iterate through PATH directories and find the executable
static char *search_in_paths(char *cmd, char **paths)
{
    char    *full_path;
    int     i;

    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (!full_path)
            return (NULL);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path); // Free path if not found/executable
        i++;
    }
    return (NULL); // Not found in any path
}

// --- Main find_executable function ---

char    *find_executable(char *cmd, t_env *env)
{
    char    *path_env;
    char    **paths;
    char    *found_path;

	if (!cmd || !*cmd)
		return (NULL); // Return NULL if cmd is empty

    if (ft_strchr(cmd, '/'))
        return (handle_direct_path(cmd));

    path_env = my_getenv("PATH", env);
    if (!path_env)
        return (NULL);

    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    found_path = search_in_paths(cmd, paths);

    free_str_array(paths); // Always free paths array after use
    return (found_path);
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
		if (ft_strcmp(current->key, key) == 0) 
		{
			return current->value; // 返回环境变量的值
		}
		current = current->next;
	}
	return NULL; // 如果没有找到，返回NULL
}

/* Free command arguments array */
static void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/* Free redirection list */
static void	free_redirections(redir_t *redir)
{
	redir_t	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_content)
			free(redir->heredoc_content);
		free(redir);
		redir = next;
	}
}

/* Free command node data */
static void	free_command_node(ast_node_t *node)
{
	free_command_args(node->data.command.args);
	free_redirections(node->data.command.redirs);
}

/* Free pipe node data */
static void	free_pipe_node(ast_node_t *node)
{
	free_ast(node->data.binary.left);
	free_ast(node->data.binary.right);
}

/* Free AST node recursively */
void	free_ast(ast_node_t *node)
{
	if (!node)
		return ;
	if (node->type == AST_COMMAND)
		free_command_node(node);
	else if (node->type == AST_PIPE)
		free_pipe_node(node);
	free(node);
}

