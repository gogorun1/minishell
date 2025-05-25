#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# define _DEFAULT_SOURCE

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

/*
** redirect_in: <
** redirect_out: >
** heredoc: <<
** append: >>
** pipe: |
** word: any other string
*/

typedef enum e_token_type
{
	TOKEN_WORD,			// A sequence of characters that form a word
	TOKEN_PIPE,			// A pipe (|) indicating a command separator
	TOKEN_REDIRECT_IN,	// A less-than sign (<) indicating input redirection
	TOKEN_REDIRECT_OUT, // A greater-than sign (>) indicating output redirection
	TOKEN_APPEND,	 	// A double greater-than sign (>>) indicating append output redirection
	TOKEN_HEREDOC,		// A double less-than sign (<<) indicating here-document input redirection
	TOKEN_EOF,			// End of file or end of input
	TOKEN_ERROR			// An error token indicating a parsing error
	// TOKEN_SQUOTE,		// A single quote (') indicating a single-quoted string
	// TOKEN_DQUOTE,		// A double quote (") indicating a double-quoted string
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;	// The type of the token
	char			*value;	// The value of the token
	struct s_token	*next;	// Pointer to the next token in the list
}	t_token;

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	int		redirect_in;
	int		redirect_out;
	int		heredoc;
	int		append;
	struct s_cmd *next;
}	t_cmd;

typedef enum {
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} redir_type_t;

typedef struct redir {
    redir_type_t type;
    char *file;
    int fd;
    struct redir *next;
} redir_t;

typedef struct {
    char **args;
    redir_t *redirs;
} command_t;

typedef enum {
    AST_COMMAND,
    AST_PIPE,
    AST_AND,
    AST_OR
} ast_node_type_t;

typedef struct ast_node {
    ast_node_type_t type;
    union {
        command_t command;
        struct {
            struct ast_node *left;
            struct ast_node *right;
        } binary;
    } data;
} ast_node_t;

typedef struct {
    t_token *tokens;
    t_token *current;
} parser_t;

/*
** key   : the environment variable name (e.g., "PATH", "HOME", "USER")
** value : the environment variable value (e.g., "/usr/bin", "/home/lcao", "lcao")
*/
typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*init_env(char **envp);


// Function prototypes
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **head, t_token *new_token);
void	free_token(t_token *token);
void	free_token_list(t_token *head);
t_token	*tokenizer(char *line);
void	print_tokens(t_token *tokens);
void	handle_special_char(char *line, int *i, t_token **tokens);
char	*find_executable(char *cmd);
char	*get_env_value(char *var_name);
char	*expand_variables(char *str);
int		handle_quotes(char **input, int *i, t_token **tokens);
char	*ft_strndup(const char *s, size_t n);

/*builtin*/
int	builtin_cd(char **args);
int	builtin_pwd(void);
int	builtin_exit(char **args);
int	builtin_echo(char **args);
int	builtin_env_list(t_env *env);
int	builtin_export(char **args, t_env **env);

/*env*/
int	builtin_unset(char **args, t_env **env_list);
t_env	*init_env(char **envp);

int	is_builtin(char *cmd);
int	run_builtin(char **args, t_env **env_list);

/*utils*/
char	*ft_strndup(const char	*s, size_t n);

char	**split_input(char *input);

/*execution*/

#endif