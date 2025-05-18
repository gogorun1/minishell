/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:50:50 by wding             #+#    #+#             */
/*   Updated: 2025/05/18 17:54:04 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
}	t_cmd;

/*builtin*/
int	builtin_cd(char **args);
int	builtin_pwd(void);
int	builtin_exit(char **args);
int	builtin_env(char **envp);

int	is_builtin(char *cmd);
int	run_builtin(char **args, char **envp);

char	**split_input(char *input);

#endif