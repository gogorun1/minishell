/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:47:30 by lcao              #+#    #+#             */
/*   Updated: 2025/06/15 17:44:29 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*void	execute_child(char *path, char **args, t_env *env)
{
	char	**envp;

	envp = env_to_array(env);
	if (!envp)
		exit(127);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, args, envp);
	perror(path);
	free_env_array(envp);
	exit(127);
}*/

int	save_stdio(int saved_fds[2])
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
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

int	wait_for_child(pid_t pid)
{
// In your execute_ast or wherever you wait for child processes
	int status;
	pid_t result = waitpid(pid, &status, 0);

	if (result > 0)
	{
    	if (WIFSIGNALED(status))
    	{
        	int signal_num = WTERMSIG(status);
        	if (signal_num == SIGINT)
        	{
				write(STDOUT_FILENO, "\n", 1);
				return 130;  // Standard exit code for SIGINT
			}
			else if (signal_num == SIGQUIT)
			{
				printf("Quit\n");  // bash prints "Quit" for SIGQUIT
				return 131;  // Standard exit code for SIGQUIT
			}
		}
		else if (WIFEXITED(status))
		{
			return WEXITSTATUS(status);
		}
	}
	return (-1);
}

int	execute_external(command_t *cmd, t_shell *shell)
{
	char	*path;
	pid_t	pid;
	int		saved_fds[2];
	int		result;

	path = find_executable(cmd->args[0], shell->env_list);
	if (!path)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			cmd->args[0]);
		return (127);
	}
	if (save_stdio(saved_fds) == -1)
		return (cleanup_external(path, NULL, -1));
	if (cmd->redirs && setup_redirections(cmd->redirs) != 0)
		return (cleanup_external(path, saved_fds, 1));
	pid = fork();
	if (pid == -1)
		return (cleanup_external(path, saved_fds, -1));
	if (pid == 0)
		run_external_command_in_child(path, cmd->args, shell->env_list);
	result = wait_for_child(pid);
	cleanup_external(path, saved_fds, 0);
	return (result);
}
