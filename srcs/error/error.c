/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:44:42 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 14:56:25 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_heredoc(char *token)
{
	ft_putstr_fd("Minishell: warning: here-document delimited by eof (wanted `",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

void	error_token(char *token)
{
	ft_putstr_fd("Minishell: syntax error near unexpected token '",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

int	perror_r(char *error_type, char *token)
{
	ft_putstr_fd(error_type, STDERR_FILENO);
	perror(token);
	return (-1);
}
