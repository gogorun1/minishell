/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:30:06 by lcao              #+#    #+#             */
/*   Updated: 2025/05/24 19:14:13 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*test if it is builtin*/
#include "minishell.h"

int is_builtin(char *cmd)
{
	if(!cmd)
		return (0);
	if(strcmp(cmd, "cd") == 0)
		return (1);
	if(strcmp(cmd, "exit") == 0)
		return (1);
	if(strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "echo") == 0)
		return (1);
	return (0);
}

/*based on the name to run those builtins*/
int	run_builtin(char *arg, t_env **env_list)
{
	if(!arg || !arg)
		return (1);
	if(strcmp(arg, "cd") == 0)
		return(builtin_cd(arg));
	if(strcmp(arg, "exit") == 0)
		return(builtin_exit(arg));
	if(strcmp(arg, "pwd") == 0)
		return(builtin_pwd());
	if (strcmp(arg, "env") == 0)
		builtin_env_list(*env_list);
	if (strcmp(arg, "unset") == 0)
		return (builtin_unset(arg, env_list));
	if (strcmp(arg, "export") == 0)
		return (builtin_export(arg, env_list));
	if (strcmp(arg, "echo") == 0)
		return (builtin_echo(arg));
	return (0);
}
