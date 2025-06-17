/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:09:03 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 14:57:42 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handle append redirection (>>)
int	handle_append_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

// Handle heredoc redirection (<<)
int	handle_heredoc_redirect(char *content)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	write(pipefd[1], content, strlen(content));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

// Write heredoc line to pipe
int	write_heredoc_line(char *line, int write_fd)
{
	if (write(write_fd, line, strlen(line)) == -1)
	{
		perror("write");
		return (1);
	}
	if (write(write_fd, "\n", 1) == -1)
	{
		perror("write");
		return (1);
	}
	return (0);
}
