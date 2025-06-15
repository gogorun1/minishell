/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:43:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/15 18:16:49 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args, t_shell *shell)
{
	const char	*path;

	if (args[1] && args[2])
	{
		error_cd_too_many_args(shell);
		return (1);
	}
	if (!args[1])
	{
		path = my_getenv("HOME", shell->env_list);
		if (!path)
			return (error_cd_home_not_set(shell), 1);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		error_cd(path, shell);
		return (1);
	}
	if (shell)
		shell->last_exit_status = 1;
	return (0);
}
