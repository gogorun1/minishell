/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:43:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/03 16:43:55 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args, t_shell *shell)
{
	const char	*path;

	if (args[1] && args[2]) {
		ft_fprintf(2, "minishell: cd: too many arguments\n");
		if (shell) shell->last_exit_status = 1;
		return 1;
	}
	if (!args[1]) {
		path = getenv("HOME");
		if (!path) {
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			if (shell) shell->last_exit_status = 1;
			return 1;
		}
	} else {
		path = args[1];
	}
	if (chdir(path) != 0) {
		ft_fprintf(2, "minishell: cd: %s: %s\n", path, strerror(errno));
		if (shell) shell->last_exit_status = 1;
		return 1;
	}
	if (shell) shell->last_exit_status = 0;
	return 0;
}

