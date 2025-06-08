/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abby <abby@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:07:42 by lcao              #+#    #+#             */
/*   Updated: 2025/06/08 13:40:41 by abby             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Execute pipeline
int	execute_pipeline(ast_node_t *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	left_pid = fork();
	if (left_pid == 0)
		execute_left_pipe(node, pipe_fd, shell);
	else if (left_pid < 0)
		return (handle_pipe_fork_error(pipe_fd));
	right_pid = fork();
	if (right_pid == 0)
		execute_right_pipe(node, pipe_fd, shell);
	else if (right_pid < 0)
		return (handle_pipe_fork_error_right(pipe_fd, left_pid));
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_for_pipeline(left_pid, right_pid));
}

// Execute left side of pipe
void	execute_left_pipe(ast_node_t *node, int pipe_fd[2], t_shell *shell)
{
	signal(SIGINT, SIG_DFL);  // Reset to default signal handling in child
	signal(SIGQUIT, SIG_DFL);
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exit(execute_ast(node->data.binary.left, shell));
}

// Execute right side of pipe
void	execute_right_pipe(ast_node_t *node, int pipe_fd[2], t_shell *shell)
{
	signal(SIGINT, SIG_DFL);  // Reset to default signal handling in child
	signal(SIGQUIT, SIG_DFL);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exit(execute_ast(node->data.binary.right, shell));
}

// Wait for both processes in pipeline
int	wait_for_pipeline(pid_t left_pid, pid_t right_pid)
{
	int	left_status;
	int	right_status;

	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	
	// If either process was terminated by a signal, return that status
	if (WIFSIGNALED(left_status))
		return (128 + WTERMSIG(left_status));
	if (WIFSIGNALED(right_status))
		return (128 + WTERMSIG(right_status));
	
	// Otherwise return the exit status of the rightmost command
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	return (1);
}

// Handle fork error for pipeline
int	handle_pipe_fork_error(int pipe_fd[2])
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (1);
}
