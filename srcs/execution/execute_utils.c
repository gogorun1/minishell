/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:10:31 by lcao              #+#    #+#             */
/*   Updated: 2025/06/15 17:27:10 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Run external command in child process (forked)
void	run_external_command_in_child(char *path, char **args, t_env *env)
{
	char	**envp;

	// printf("it is in the child\n");
	signal(SIGINT, SIG_DFL); // Revert SIGINT to its default (terminate process)
	signal(SIGQUIT, SIG_DFL);
	envp = env_to_array(env);
	if (!envp)
		exit(1);
	if (execve(path, args, envp) == -1)
	{
		printf("path: %s, args[0]: %s\n", path, args[0]);
		perror("execve");
		free_env_array(envp);
		exit(127);
	}

}

// Restore stdin/stdout
void	restore_stdio(int saved_fds[2])
{
	if (saved_fds[0] != -1)
	{
		close(saved_fds[0]);

		dup2(saved_fds[0], STDIN_FILENO);
		
	}
	if (saved_fds[1] != -1)
	{
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[1]);
	}
}

// Handle fork error
int	handle_fork_error(char *path, char **envp)
{
	perror("fork");
	free(path);
	free_env_array(envp);
	return (1);
}

// Handle pipeline fork error with right side
int	handle_pipe_fork_error_right(int pipe_fd[2], pid_t left_pid)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	return (1);
}
