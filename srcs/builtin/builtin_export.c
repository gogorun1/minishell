/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:27:42 by lcao              #+#    #+#             */
/*   Updated: 2025/06/16 11:05:47 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* Search for a node matching key; return it or NULL */
static t_env	*find_node(const char *key, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/* Create a new node with given key/value and push to head */
static void	add_node(char *key, char *value, t_env **env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = key;
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = *env;
	*env = new;
}

static void	update_or_add(char *key, char *value, t_env **env)
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
		free(key);
		return ;
	}
	add_node(key, value, env);
}

static int	handle_export_arg_with_equal(char *arg, char *eq,
	t_env **env, t_shell *shell)
{
	char	*key;
	char	*value;

	key = ft_strndup(arg, eq - arg);
	if (!is_valid_var_name(key))
	{
		export_error(arg, shell);
		free(key);
		return (1);
	}
	value = strdup(eq + 1);
	update_or_add(key, value, env);
	return (0);
}

int	builtin_export(char **args, t_env **env, t_shell *shell)
{
	int		i;
	char	*eq;
	int		has_error;

	i = 1;
	has_error = 0;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			if (handle_export_arg_with_equal(args[i], eq, env, shell))
				has_error = 1;
		}
		else if (!is_valid_var_name(args[i]))
		{
			export_error(args[i], shell);
			has_error = 1;
		}
		i++;
	}
	return (has_error);
}
