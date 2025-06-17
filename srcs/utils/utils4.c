/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:21:06 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 21:25:55 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper to handle the case where cmd contains a '/'
static char	*handle_direct_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

// Helper to iterate through PATH directories and find the executable
static char	*search_in_paths(char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// --- Main find_executable function ---

char	*find_executable(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*found_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_direct_path(cmd));
	path_env = my_getenv("PATH", env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	found_path = search_in_paths(cmd, paths);
	free_str_array(paths);
	return (found_path);
}

// Custom getenv to work with your envp array
char	*my_getenv(const char *key, t_env *envp)
{
	t_env	*current;

	if (!key || !envp)
	{
		return (NULL);
	}
	current = envp;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}
