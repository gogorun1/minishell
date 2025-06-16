/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:43:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/16 19:18:56 by lcao             ###   ########.fr       */
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
		// free(key);
		return ;
	}
	add_node(key, (char *)value, env);
}

int	builtin_cd(char **args, t_shell *shell)
{
	const char	*path;
	char buffer[2000];

	if (args[1] && args[2])
	{
		error_cd_too_many_args(shell);
		return (1);
	}
	if (!args[1])
	{
		path = my_getenv("HOME", shell->env_list);
		if (!path)
			return (error_cd_home_not_set(shell), 1);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		error_cd(path, shell);
		return (1);
	}
	getcwd(buffer, 2000);
	char *oldpwd = ft_strdup("OLDPWD");
	update_or_add(oldpwd, my_getenv("PWD", shell->env_list), &shell->env_list);
	char *pwd = ft_strdup("PWD");
	update_or_add(pwd, buffer, &shell->env_list);
	// update_env("OLDPWD", my_getenv("PWD",shell->env_list), &shell->env_list);
	// update_env("PWD", buffer, &shell->env_list);
	if (shell)
		shell->last_exit_status = 1;
	return (0);
}
