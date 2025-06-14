/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                   :+: :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:00:00 by lcao              #+#    #+#             */
/*   Updated: 2025/06/13 00:00:00 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirections(redir_t *redirs)
{
	redir_t	*current;
	int		result;

	current = redirs;
	while (current)
	{
		result = handle_single_redirect(current);
		if (result != 0)
			return (result);
		current = current->next;
	}
	return (0);
}

int	handle_single_redirect(redir_t *redir)
{
	if (!redir)
		return (-1);
	if (redir->type == REDIR_IN)
		return (handle_input_redirect(redir->file));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redirect(redir->file));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redirect(redir->file));
	else if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc_redirect(redir->file));
	return (-1);
}

int	handle_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_append_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}