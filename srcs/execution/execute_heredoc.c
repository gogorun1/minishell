/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:38:28 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 22:54:04 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* In your execution function, use this for heredoc: */
void	handle_heredoc_in_execution(t_redir *redir)
{
	if (redir->type == REDIR_HEREDOC)
	{
		setup_heredoc_for_execution(redir);
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
	}
}
