/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:11:29 by lcao              #+#    #+#             */
/*   Updated: 2025/06/15 19:55:01 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirections(redir_t *redirs);
int	handle_single_redirect(redir_t *redir);
int	handle_input_redirect(char *filename);
int	handle_output_redirect(char *filename);

// Setup all redirections for a command
int	setup_redirections(redir_t *redirs)
{
	redir_t	*current;

	current = redirs;
	while (current)
	{
		// write(2, "setup_redirections\n", 19);
		if (handle_single_redirect(current) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}

// Handle a single redirection
int	handle_single_redirect(redir_t *redir)
{
	if (redir->type == REDIR_IN)
		return (handle_input_redirect(redir->file));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redirect(redir->file));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redirect(redir->file));
	else if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc_redirect(redir->heredoc_content));
	return (1);
}

// Handle input redirection (<)
int	handle_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

// Handle output redirection (>)
int	handle_output_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
