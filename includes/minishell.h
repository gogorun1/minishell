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
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/errno.h>
# include <errno.h>

# include <sys/types.h>

/* GLOBAL VARIABLE*/
extern volatile sig_atomic_t g_signal_status;
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

// typedef struct s_cmd
// {
// 	char	*cmd;
// 	char	**args;
// 	int		redirect_in;
// 	int		redirect_out;
// 	int		heredoc;
// 	int		append;
// 	struct s_cmd *next;
// }	t_cmd;

typedef enum {
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} redir_type_t;

// typedef struct redir {
//     redir_type_t type;
//     char *file;
//     // int fd; // 在parser中不需要fd，执行时才需要
//     struct redir *next;
// } redir_t;


// 14 jun----------------------------------------------------
// Add these fields to your redir_t struct in header file:
typedef struct s_redir {
	redir_type_t	type;
	char			*file;
	char			*heredoc_content;  // Store heredoc content
	int				heredoc_fd;        // Pipe fd for execution
	struct s_redir	*next;
}	redir_t;

/* Add to your header file (minishell.h) */
typedef struct s_heredoc_data {
	char	*delimiter;
	char	*content;	// Store content as string
	int		processed;
}	t_heredoc_data;
//14 jun---------------------------------------------------------


typedef struct {
    char **args;
    redir_t *redirs;
} command_t;

typedef enum {
    AST_COMMAND,
    AST_PIPE
} ast_node_type_t;

typedef struct ast_node {
    ast_node_type_t type;
    union 
	{
        command_t command;
        struct 
		{
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

// --- Main Shell State Struct ---
// Encapsulates all the shell's runtime data
typedef struct s_shell {
    t_env   *env_list;         // Head of the linked list of environment variables
    int         last_exit_status;  // Value for $?
    // You might add other fields here as needed:
    // char        *current_pwd;     // Current working directory (for efficiency, or getcwd)
    // t_history   *history_data;    // If you manage history beyond readline's built-in
    // ... any other global-like state
} t_shell;


// Function prototypes
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **head, t_token *new_token);
void	free_token(t_token *token);
void	free_token_list(t_token *head);
t_token	*tokenizer(char *line, t_shell *g_shell);
void	print_tokens(t_token *tokens);
void	handle_special_char(char *line, int *i, t_token **tokens);
char	*find_executable(char *cmd, t_env *env);
char	*get_env_value(char *var_name);
// int		handle_quotes(char **input, int *i, t_token **tokens);
char	*ft_strndup(const char *s, size_t n);
bool is_special_char(char c);
void print_token(t_token *token);
void print_ast(ast_node_t *node, int indent_level);
void free_ast(ast_node_t *node);
ast_node_t *parse_command(parser_t *parser);
ast_node_t *parse_pipeline(parser_t *parser);
ast_node_t *parse(t_token *tokens);
char	*expand_variables(char *str, t_shell *g_shell);
void setup_execution_signals(void);
char	*read_heredoc_content(char *delimiter);
char *ft_strcat(char *dest, const char *src);
char *ft_strjoin3(const char *s1, const char *s2, const char *s3);
int	ft_strcmp(const char *s1, const char *s2);
void free_str_array(char **arr);

/*builtin*/
int	builtin_cd(char **args, t_shell *shell);
int	builtin_pwd(void);
int	builtin_exit(char **args, t_shell *shell);
int	builtin_echo(char **args);
int	builtin_env_list(t_env *env, t_shell *shell, char **args);
int	builtin_export(char **args, t_env **env, t_shell *shell);
int export_error(const char *msg, t_shell *shell);

/*env*/
int	builtin_unset(char **args, t_env **env_list);
int	is_valid_var_char(char c);
int	is_valid_var_name(char *s);
int	print_env_list(t_env *env);
t_env	*init_env(char **envp);
char *my_getenv(const char *key, t_env *envp);


int	is_builtin(char *cmd);
int	run_builtin(char **args, t_env **env_list, t_shell *shell);

/*utils*/
char	*ft_strndup(const char	*s, size_t n);
char	**split_input(char *input);
int     ft_fprintf(int fd, const char *format, ...);

/* execution */
int		execute_ast(ast_node_t *node, t_shell *shell);
int		execute_command(command_t *cmd, t_shell *shell);
int		execute_external(command_t *cmd, t_shell *shell);
int		execute_pipeline(ast_node_t *node, t_shell *shell);
void	execute_left_pipe(ast_node_t *node, int pipe_fd[2], t_shell *shell);
void	execute_right_pipe(ast_node_t *node, int pipe_fd[2], t_shell *shell);
void	run_external_command_in_child(char *path, char **args, t_env *env);
void	restore_stdio(int saved_fds[2]);


int     wait_and_get_status(pid_t pid, char *path, char **envp);
int     handle_fork_error(char *path, char **envp);

/*execute pipelines*/
int handle_pipe_fork_error(int pipe_fd[2]);
int handle_pipe_fork_error_right(int pipe_fd[2], pid_t left_pid);
int wait_for_pipeline(pid_t left_pid, pid_t right_pid);


/* redirections */
int		setup_redirections(redir_t *redirs);
int		handle_single_redirect(redir_t *redir);
int		handle_input_redirect(char *filename);
int		handle_output_redirect(char *filename);
int		handle_append_redirect(char *filename);
int		handle_heredoc_redirect(char *delimiter);
// int		read_heredoc_input(char *delimiter, int write_fd);
int		write_heredoc_line(char *line, int write_fd);

/* env utils */

char	*create_env_string(char *key, char *value);
void	free_env(t_env *head);

/*exeute_env*/
int		count_env_vars(t_env *env);
char	**env_to_array(t_env *env);
void	free_env_array(char **envp);
void	free_env_array_partial(char **envp, int count);

/* signals */
void	signal_handler(int sig);
void	setup_signal_handlers(void);

/* expansion */
char	*get_variable_value(const char *var_name_start, int var_len, t_shell *shell);
char	*expand_variables(char *str, t_shell *g_shell);
int		ft_var_name_len(const char *s);

/*error*/
void	error_msg(char *command, char *message, int exit_code, t_shell *shell);
void	error_cd(const char *path, t_shell *shell);
void	error_cd_too_many_args(t_shell *shell);
void	error_cd_home_not_set(t_shell *shell);

#endif