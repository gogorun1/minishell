#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <stdbool.h>


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

// typedef struct s_cmd
// {
// 	char		**args;	// The arguments for the command
// 	int			in_fd;	// File descriptor for input redirection
// 	int			out_fd; // File descriptor for output redirection
// 	int			append_mode; // Flag for append mode
// 	char		*heredoc; // Here-document content
// 	struct s_cmd	*next;	// Pointer to the next command in the pipeline
// }	t_cmd;

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

// Function prototypes
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **head, t_token *new_token);
void	free_token(t_token *token);
void	free_token_list(t_token *head);
t_token	*tokenizer(char *line);
void	print_tokens(t_token *tokens);
bool	is_valid_var_char(char c);
bool	is_special_char(char c);
void	handle_special_char(char *line, int *i, t_token **tokens);
char	*find_executable(char *cmd);
char	*get_env_value(char *var_name);
char	*expand_variables(char *str);
int		handle_quotes(char **input, int *i, t_token **tokens);
char	*ft_strndup(const char *s, size_t n);

#endif