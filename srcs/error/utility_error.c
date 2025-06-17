/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:09:29 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 21:00:05 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check and print system error with context */
int	check_system_error(int result, char *context, char *command)
{
	if (result == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (context)
		{
			ft_putstr_fd(context, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
		}
		perror(command);
		return (-1);
	}
	return (result);
}

/* Enhanced syntax error with better context */
void	error_syntax_context(char *context, char *token)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (context)
	{
		ft_putstr_fd(context, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	if (token)
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/* Generic error with errno */
void	error_errno(char *command, char *context, t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	perror(context);
	if (shell)
	{
		if (errno == ENOENT)
			shell->last_exit_status = 127;
		else if (errno == EACCES)
			shell->last_exit_status = 126;
		else
			shell->last_exit_status = 1;
	}
}
