/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:12:44 by lcao              #+#    #+#             */
/*   Updated: 2025/06/03 15:34:20 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* CD errors */
void	error_cd(char *path, t_shell *shell)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path);
	if (shell)
		shell->last_exit_status = 1;
}

/* Export errors */
void	error_export(char *identifier, t_shell *shell)
{
	error_msg("export", "not a valid identifier", 1, shell);
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/* Exit errors */
void	error_exit(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	if (shell)
		shell->last_exit_status = 2;
}

// 通用 builtin_error 实现
int builtin_error(const char *msg, t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (shell)
		shell->last_exit_status = 1;
	return (1);
}
