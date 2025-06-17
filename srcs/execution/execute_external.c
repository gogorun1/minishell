/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:47:30 by lcao              #+#    #+#             */
/*   Updated: 2025/06/18 00:35:24 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_stdio(int saved_fds[2])
{
	if (saved_fds[0] == -1 || saved_fds[1] == -1)
	{
		perror("dup");
		return (-1);
	}
	return (0);
}

int	cleanup_external(char *path, int saved_fds[2], int error_code)
{
	restore_stdio(saved_fds);
	free(path);
	if (error_code == -1)
		perror("fork");
	return (error_code);
}

/*In your execute_ast or wherever you wait for child processes*/
int	wait_for_child(pid_t pid)
{
	int		status;
	pid_t	result;
	int		signal_num;

	result = waitpid(pid, &status, 0);
	if (result > 0)
	{
		if (WIFSIGNALED(status))
		{
			signal_num = WTERMSIG(status);
			if (signal_num == SIGINT)
			{
				write(STDOUT_FILENO, "\n", 1);
				return (130);
			}
			else if (signal_num == SIGQUIT)
			{
				printf("Quit\n");
				return (131);
			}
		}
		else if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (-1);
}

static int	handle_command_not_found(t_command *cmd, t_shell *shell)
{
	ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n",
		cmd->args[0]);
	if (shell)
	{
		shell->last_exit_status = 127;
	}
	return (127);
}

int	execute_external(t_command *cmd, t_shell *shell, int saved_fds[2])
{
	char	*path;
	pid_t	pid;
	int		result;

	path = find_executable(cmd->args[0], shell->env_list);
	if (!path)
		return (handle_command_not_found(cmd, shell));
	if (save_stdio(saved_fds) == -1)
		return (cleanup_external(path, NULL, -1));
	if (cmd->redirs && setup_redirections(cmd->redirs) != 0)
		return (cleanup_external(path, saved_fds, 1));
	pid = fork();
	if (pid == -1)
		return (cleanup_external(path, saved_fds, -1));
	if (pid == 0)
	{
		close(saved_fds[0]);
		close(saved_fds[1]);
		run_external_command_in_child(path, cmd->args, shell->env_list);
	}
	result = wait_for_child(pid);
	cleanup_external(path, saved_fds, 0);
	return (result);
}
