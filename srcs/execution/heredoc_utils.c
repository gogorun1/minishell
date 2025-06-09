/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:09:03 by lcao              #+#    #+#             */
/*   Updated: 2025/06/09 16:47:37 by lcao             ###   ########.fr       */
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
int	handle_heredoc_redirect(char *delimiter)
{
	int	pipe_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	if (read_heredoc_input(delimiter, pipe_fd[1]) != 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipe_fd[0]);
		return (1);
	}
	close(pipe_fd[0]);
	return (0);
}

// Read heredoc input from user
int	read_heredoc_input(char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			// ft_fprintf(STDERR_FILENO,
			// 	"minishell: warning: here-document delimited by end-of-file\n");
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write_heredoc_line(line, write_fd) != 0)
		{
			free(line);
			return (1);
		}
		free(line);
	}
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
