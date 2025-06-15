/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:01:24 by lcao              #+#    #+#             */
/*   Updated: 2025/06/15 20:02:53 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* CD errors */
void	error_cd(const char *path, t_shell *shell)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path);
	if (shell)
		shell->last_exit_status = 1;
}

void	error_cd_too_many_args(t_shell *shell)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
	if (shell)
		shell->last_exit_status = 1;
}

void	error_cd_home_not_set(t_shell *shell)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	if (shell)
		shell->last_exit_status = 1;
}
