/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:23:57 by wding             #+#    #+#             */
/*   Updated: 2025/06/18 04:50:50 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# define _DEFAULT_SOURCE

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* GLOBAL VARIABLE*/
extern volatile sig_atomic_t	g_signal_status;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_ERROR
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef enum s_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_ERR
}								t_redir_type;

// 14 jun---------------------------------------------------
typedef struct s_redir
{
	t_redir_type				type;
	char						*file;
	char						*heredoc_content;
	int							heredoc_fd;
	struct s_redir				*next;
}								t_redir;

/* Add to your header file (minishell.h) */
typedef struct s_heredoc_data
{
	char						*delimiter;
	char						*content;
	int							processed;
}								t_heredoc_data;

typedef struct s_command
{
	char						**args;
	t_redir						*redirs;
}								t_command;

typedef enum s_ast_node_type
{
	AST_COMMAND,
	AST_PIPE
}								t_ast_node_type;

typedef struct s_ast_node
{
	t_ast_node_type				type;
	union
	{
		t_command				command;
		struct
		{
			struct s_ast_node		*left;
			struct s_ast_node		*right;
		} s_binary;
	} u_data;
}								t_ast_node;

typedef struct parser_s
{
	t_token						*tokens;
	t_token						*current;
}								t_parser;

/*
** key   : the environment variable name (e.g., "PATH", "HOME", "USER")
** value : the environment variable value (e.g., "/usr/bin", "/home/lcao",
	"lcao")
*/
typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

// --- Main Shell State Struct ---
// Encapsulates all the shell's runtime data
typedef struct s_shell
{
	t_env						*env_list;
	int							last_exit_status;
}								t_shell;

typedef struct s_token_data
{
	t_shell						*shell;
	char						*word;
	int							in_word;
}								t_token_data;

typedef struct s_expand_data
{
	int							i;
	int							start;
	char						*result;
}								t_expand_data;

typedef struct s_cleanup
{
	t_token						*tokens;
	t_ast_node					*ast;
	char						*input;
}								t_cleanup;

typedef struct s_exec_data
{
	char						*input;
	t_token						*tokens;
	t_ast_node					*ast;
}								t_exec_data;

// Function prototypes
t_token							*tokenizer(char *line, t_shell *g_shell);
void							print_tokens(t_token *tokens);
void							handle_special_char(char *line, int *i,
									t_token **tokens);
char							*find_executable(char *cmd, t_env *env);
char							*get_env_value(char *var_name);
char							*ft_strndup(const char *s, size_t n);
bool							is_special_char(char c);
void							print_token(t_token *token);
void							print_ast(t_ast_node *node, int indent_level);
void							free_ast(t_ast_node *node);
t_ast_node						*parse_command(t_parser *parser);
t_ast_node						*parse_pipeline(t_parser *parser);
t_ast_node						*parse(t_token *tokens);
char							*expand_variables(char *str, t_shell *g_shell);
void							setup_execution_signals(void);
char							*read_heredoc_content(char *delimiter);
char							*ft_strcat(char *dest, const char *src);
char							*ft_strjoin3(const char *s1, const char *s2,
									const char *s3);
int								ft_strcmp(const char *s1, const char *s2);
void							free_str_array(char **arr);
void							cleanup_and_exit(t_shell *shell,
									t_token *tokens, t_ast_node *ast,
									char *input);

/*--------------------------------tokenizer-------------------------------*/

/* token_utils.c */
t_token							*create_token(char *str, t_token_type type);
void							add_token(t_token **head, t_token *new_token);
void							free_token(t_token *token);
void							free_token_list(t_token *head);

/* token_quote.c */
int								tokenizer_handle_quote(const char *line, int *i,
									t_token_data *data);

/* token_word.c */
int								tokenizer_handle_word(const char *line, int *i,
									t_token_data *data);

/* tokenizer_main.c */
t_token							*tokenizer(char *line, t_shell *shell);

/* ---------------------------------Parsing------------------------------- */
/* redirection_utils.c */
void							add_redirection(t_command *command_data,
									t_redir_type type, char *filename);
int								process_heredoc_during_parse(t_redir *redir);
t_redir							*find_last_redirection(t_ast_node *node);
int								process_heredoc_redirection(t_ast_node *node,
									t_redir_type type);
int								process_command_heredocs_during_parse(
									t_command *cmd);

/* heredoc_reader.c */
char							*read_heredoc_content(char *delimiter);
void							handle_heredoc_eof_warning(char *delimiter);
int								process_heredoc_line(char *line,
									char *delimiter, char *buffer);
int								setup_heredoc_for_execution(t_redir *redir);
void							handle_heredoc_in_execution(t_redir *redir);

/* parser_utils.c */
int								is_redirection_token(t_token_type type);
t_redir_type					get_redirection_type(t_token_type token_type);
t_ast_node						*init_command_node(void);
char							**handle_word_token(t_parser *parser,
									char **args, int *arg_count);
t_ast_node						*create_pipe_node(t_ast_node *left,
									t_ast_node *right);

/* parser_handlers.c */
int								handle_redirection(t_parser *parser,
									t_ast_node *node);
int								handle_word_in_parse(t_parser *parser,
									char ***args, int *arg_count);
int								handle_redir_in_parse(t_parser *parser,
									t_ast_node *node);
int								process_command_tokens(t_parser *parser,
									t_ast_node *node, char ***args,
									int *arg_count);
int								handle_heredoc_sigquit(int *pipefd);
int								check_pipe_syntax_errors(t_parser *parser);

/* parser_main.c */
t_ast_node						*parse_command(t_parser *parser);
t_ast_node						*parse_pipeline(t_parser *parser);
t_ast_node						*parse(t_token *tokens);

/* ------------------------------------------------------------------------*/
/* Function prototypes to add */
char							*get_variable_value(const char *var_name_start,
									int var_len, t_shell *shell);
int								ft_var_name_len(const char *s);
char							*expand_variables(char *str, t_shell *shell);
char							*join_and_free(char *result, char *to_add);
void							add_literal_part(t_expand_data *data,
									const char *str);
void							handle_exit_status(t_expand_data *data,
									t_shell *shell);
void							handle_variable(t_expand_data *data,
									const char *str, t_shell *shell);

/*builtin*/
int								builtin_cd(char **args, t_shell *shell);
int								builtin_pwd(void);
int								builtin_exit(char **args, t_shell *shell);
int								builtin_echo(char **args);
int								builtin_env_list(t_env *env, t_shell *shell,
									char **args);
int								builtin_export(char **args, t_env **env,
									t_shell *shell);
int								export_error(const char *msg, t_shell *shell);
void							update_or_add(char *key, char *value,
									t_env **env);
t_env							*find_node(const char *key, t_env *env);
void							add_node(char *key, char *value, t_env **env);

/*env*/
int								builtin_unset(char **args, t_env **env_list);
int								is_valid_var_char(char c);
int								is_valid_var_name(char *s);
int								print_env_list(t_env *env);
t_env							*init_env(char **envp);
char							*my_getenv(const char *key, t_env *envp);

int								is_builtin(char *cmd);
int								run_builtin(char **args, t_env **env_list,
									t_shell *shell);

/*utils*/
char							*ft_strndup(const char *s, size_t n);
char							**split_input(char *input);
int								ft_fprintf(int fd, const char *format, ...);

/* execution */
int								execute_ast(t_ast_node *node, t_shell *shell);
int								execute_command(t_ast_node *node, t_shell *shell);
int								execute_external(t_command *cmd, t_shell *shell,
									int saved_fds[2]);
int								execute_pipeline(t_ast_node *node,
									t_shell *shell);
void							execute_left_pipe(t_ast_node *node,
									int pipe_fd[2], t_shell *shell);
void							execute_right_pipe(t_ast_node *node,
									int pipe_fd[2], t_shell *shell);
void							run_external_command_in_child(char *path,
									char **args, t_env *env);
void							restore_stdio(int saved_fds[2]);

int								wait_and_get_status(pid_t pid, char *path,
									char **envp);
int								handle_fork_error(char *path, char **envp);
void							cleanup_and_exit(t_shell *shell,
									t_token *tokens, t_ast_node *ast,
									char *input);

/*execute pipelines*/
int								handle_pipe_fork_error(int pipe_fd[2]);
int								handle_pipe_fork_error_right(int pipe_fd[2],
									pid_t left_pid);
int								wait_for_pipeline(pid_t left_pid,
									pid_t right_pid);

/* redirections */
int								setup_redirections(t_redir *redirs);
int								handle_single_redirect(t_redir *redir);
int								handle_input_redirect(char *filename);
int								handle_output_redirect(char *filename);
int								handle_append_redirect(char *filename);
int								handle_heredoc_redirect(char *delimiter);
int								write_heredoc_line(char *line, int write_fd);

/* env utils */

char							*create_env_string(char *key, char *value);
void							free_env(t_env *head);

/*exeute_env*/
int								count_env_vars(t_env *env);
char							**env_to_array(t_env *env);
void							free_env_array(char **envp);
void							free_env_array_partial(char **envp, int count);

/* signals */
void							signal_handler(int sig);
void							setup_signal_handlers(void);

/* expansion */
char							*get_variable_value(const char *var_name_start,
									int var_len, t_shell *shell);
char							*expand_variables(char *str, t_shell *g_shell);
int								ft_var_name_len(const char *s);

/*error*/
void							error_msg(char *command, char *message,
									int exit_code, t_shell *shell);
void							error_cd(const char *path, t_shell *shell);
void							error_cd_too_many_args(t_shell *shell);
void							error_cd_home_not_set(t_shell *shell);
void							error_syntax_context(char *context,
									char *token);

/* main.c */
int								event(void);

/* shell_init.c */
// int								init_shell(t_shell *shell, char **envp);
// void							cleanup_and_exit_main(t_shell *shell,
									// t_cleanup *cleanup);

/* main_loop.c */
// void							run_main_loop(t_shell *shell, int input_status);
// void							handle_sigint_in_main(t_shell *shell);

#endif