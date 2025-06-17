/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:00:00 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 21:32:43 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(ast_node_t *node, t_shell *shell);
int	execute_command(command_t *cmd, t_shell *shell);
int	handle_signal_status(int status);
int	wait_and_get_status(pid_t pid, char *path, char **envp);

int	execute_ast(ast_node_t *node, t_shell *shell)
{
	int	temp;

	temp = 0;
	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
	{
		temp = execute_command(&node->data.command, shell);
		if (temp == -1)
			cleanup_and_exit(shell, NULL, node, NULL);
		return (temp);
	}
	else if (node->type == AST_PIPE)
		return (execute_pipeline(node, shell));
	return (-1);
}

// Execute a single command with redirections
int	execute_command(command_t *cmd, t_shell *shell)
{
	int	saved_fds[2];
	int	result;

	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirs) != 0)
	{
		restore_stdio(saved_fds);
		return (1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		restore_stdio(saved_fds);
		return (0);
	}
	if (is_builtin(cmd->args[0]))
		result = run_builtin(cmd->args, &(shell->env_list), shell);
	else
		result = execute_external(cmd, shell, saved_fds);
	restore_stdio(saved_fds);
	return (result);
}

int	handle_signal_status(int status)
{
	int	signal_num;

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
			printf("Exit\n");
			return (131);
		}
		return (128 + signal_num);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	wait_and_get_status(pid_t pid, char *path, char **envp)
{
	int		status;
	pid_t	result;

	result = waitpid(pid, &status, 0);
	free(path);
	free_env_array(envp);
	if (result > 0)
		return (handle_signal_status(status));
	return (1);
}
