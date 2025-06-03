/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:10:21 by lcao              #+#    #+#             */
/*   Updated: 2025/06/02 17:10:47 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* Enhanced error function with exit status setting */
void	error_msg(char *command, char *message, int exit_code, t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	
	if (shell)
		shell->last_exit_status = exit_code;
}

/* Command not found error */
void	error_command_not_found(char *command, t_shell *shell)
{
	error_msg(command, "command not found", 127, shell);
}

/* Permission denied error */
void	error_permission_denied(char *command, t_shell *shell)
{
	error_msg(command, "Permission denied", 126, shell);
}

/* Is a directory error */
void	error_is_directory(char *command, t_shell *shell)
{
	error_msg(command, "Is a directory", 126, shell);
}

/* File not found error */
void	error_file_not_found(char *filename, t_shell *shell)
{
	error_msg(filename, "No such file or directory", 1, shell);
}
