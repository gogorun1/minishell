/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:27:42 by lcao              #+#    #+#             */
/*   Updated: 2025/06/03 16:41:11 by lcao             ###   ########.fr       */
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
	t_env *new;
	
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
	t_env *node;
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

static int is_valid_var_name(const char *s)
{
	if (!s || !is_valid_var_char(s[0]) || s[0] == '\0' || (s[0] >= '0' && s[0] <= '9'))
		return 0;
	for (int i = 1; s[i] && s[i] != '='; i++)
	{
		if (!is_valid_var_char(s[i]) && s[i] != '_')
			return 0;
	}
	return 1;
}
/*
** builtin_export:
**   - args: ["export", "VAR1=ls", "VAR2=-l", ..., NULL]
**   - env:  pointer to your t_env * head
**
** For each argument KEY=VALUE:
**   1) split off key and value
**   2) call update_or_add(key, value, env)
** in general, export is a builtin that add a variable to 
   the environment, so that it can be accessed by child process
   ls, cat, env, echo, etc...
*/

int	builtin_export(char **args, t_env **env, t_shell *shell)
{
	int i = 1;
	char *eq;
	char *key;
	char *value;
	int has_error = 0;

	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			key = ft_strndup(args[i], eq - args[i]);
			if (!is_valid_var_name(key))
			{
				builtin_error(args[i], shell);
				free(key);
				has_error = 1;
			}
			else
			{
				value = strdup(eq + 1);
				update_or_add(key, value, env);
			}
		}
		else if (!is_valid_var_name(args[i]))
		{
			builtin_error(args[i], shell);
			has_error = 1;
		}
		i++;
	}
	return has_error;
}
