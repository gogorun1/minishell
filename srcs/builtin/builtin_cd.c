/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:43:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 15:20:46 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(char *key, const char *value, t_env **env)
{
	t_env	*node;

	node = find_node(key, *env);
	if (node)
	{
		free(node->value);
		if (value)
			node->value = ft_strdup(value);
		else
			node->value = NULL;
		return ;
	}
	add_node(key, (char *)value, env);
}

static const char	*get_cd_target(char **args, t_shell *shell)
{
	const char	*path;

	if (args[1] && args[2])
	{
		error_cd_too_many_args(shell);
		return (NULL);
	}
	if (!args[1])
	{
		path = my_getenv("HOME", shell->env_list);
		if (!path)
		{
			error_cd_home_not_set(shell);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

static int	try_chdir(const char *path, t_shell *shell)
{
	if (chdir(path) != 0)
	{
		error_cd(path, shell);
		return (1);
	}
	return (0);
}

static void	update_pwd_env(t_shell *shell)
{
	char	buffer[2000];
	char	*oldpwd;
	char	*pwd;

	getcwd(buffer, 2000);
	oldpwd = ft_strdup("OLDPWD");
	update_or_add(oldpwd, my_getenv("PWD", shell->env_list), &shell->env_list);
	pwd = ft_strdup("PWD");
	update_or_add(pwd, buffer, &shell->env_list);
}

int	builtin_cd(char **args, t_shell *shell)
{
	const char	*path;

	path = get_cd_target(args, shell);
	if (!path)
		return (1);
	if (try_chdir(path, shell))
		return (1);
	update_pwd_env(shell);
	if (shell)
		shell->last_exit_status = 1;
	return (0);
}
