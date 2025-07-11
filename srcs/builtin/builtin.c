/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:30:06 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 18:03:58 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	return (0);
}

/*based on the name to run those builtins*/
int	run_builtin(char **args, t_env **env_list, t_shell *shell)
{
	if (!args[0] || !args)
		return (1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, shell));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env_list(*env_list, shell, args));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, env_list));
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, env_list, shell));
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	return (0);
}
