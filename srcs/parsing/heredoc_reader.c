/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:41 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 23:13:41 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define HEREDOC_MAX_SIZE 4096

/* Handle end-of-file warning for heredoc */
void	handle_heredoc_eof_warning(char *delimiter)
{
	write(STDERR_FILENO,
		"minishell: warning: here-document delimited by end-of-file (wanted `",
		68);
	write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
	write(STDERR_FILENO, "')\n", 3);
}

/* Process a single line in heredoc */
int	process_heredoc_line(char *line, char *delimiter, char *buffer)
{
	if (errno == EINTR)
	{
		free(line);
		g_signal_status = 130;
		return (-1);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	ft_strlcat(buffer, line, HEREDOC_MAX_SIZE);
	ft_strlcat(buffer, "\n", HEREDOC_MAX_SIZE);
	free(line);
	return (0);
}

/* Read heredoc content interactively - THIS HAPPENS DURING PARSING */
char	*read_heredoc_content(char *delimiter)
{
	char	buffer[HEREDOC_MAX_SIZE];
	char	*line;
	int		result;

	buffer[0] = '\0';
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			handle_heredoc_eof_warning(delimiter);
			return (ft_strdup(buffer));
		}
		result = process_heredoc_line(line, delimiter, buffer);
		if (result == -1)
			return (NULL);
		if (result == 1)
			break ;
	}
	return (ft_strdup(buffer));
}

/* Handle SIGQUIT during heredoc setup */
int	handle_heredoc_sigquit(int *pipefd)
{
	if (g_signal_status == SIGQUIT)
	{
		printf("sigquit is triggered\n");
		g_signal_status = 0;
		close(pipefd[1]);
		return (0);
	}
	return (1);
}

/* EXECUTION PHASE: Create pipe from heredoc content */
int	setup_heredoc_for_execution(t_redir *redir)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (!handle_heredoc_sigquit(pipefd))
		return (0);
	write(pipefd[1], redir->heredoc_content, ft_strlen(redir->heredoc_content));
	close(pipefd[1]);
	redir->heredoc_fd = pipefd[0];
	return (0);
}
