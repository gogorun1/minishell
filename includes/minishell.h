/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:50:50 by wding             #+#    #+#             */
/*   Updated: 2025/05/24 19:51:23 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
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