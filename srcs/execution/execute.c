/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:00:00 by lcao              #+#    #+#             */
/*   Updated: 2025/06/03 15:28:55 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			execute_ast(ast_node_t *node, t_shell *shell);
int			execute_command(command_t *cmd, t_shell *shell);
int			execute_external(command_t *cmd, t_shell *shell);
int			wait_and_get_status(pid_t pid, char *path, char **envp);
static int	exec_external_fork(char *path, char **args, t_env *env_list);
//execute_ast：入口函数，识别节点类型并调用相应的执行函数。
//execute_command：处理一个普通命令的执行，包括重定向设置与内建命令判断。
//execute_external：fork 并用 execve 执行外部命令。

int	execute_ast(ast_node_t *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
		return (execute_command(&node->data.command, shell));
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
		result = execute_external(cmd, shell);
	shell->last_exit_status = result;
	restore_stdio(saved_fds);
	return (result);
}

// Execute external command (split for 42 norm)
static int	exec_external_fork(char *path, char **args, t_env *env_list)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child(path, args, env_list);
		exit(1);
	}
	else if (pid > 0)
		return (pid);
	else
		return (-1);
}

int	execute_external(command_t *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;
	int		pid;

	path = find_executable(cmd->args[0]);
	if (!path)
	{
		ft_fprintf(2, "minishell: %s: command not found\n",
			cmd->args[0]);
		return (127);
	}
	envp = env_to_array(shell->env_list);
	if (!envp)
	{
		free(path);
		return (1);
	}
	pid = exec_external_fork(path, cmd->args, shell->env_list);
	if (pid > 0)
		return (wait_and_get_status(pid, path, envp));
	else
		return (handle_fork_error(path, envp));
}

// Wait for child and get exit status
int	wait_and_get_status(pid_t pid, char *path, char **envp)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	free(path);
	free_env_array(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
