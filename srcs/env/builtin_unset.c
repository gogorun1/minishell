/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:04:03 by lcao              #+#    #+#             */
/*   Updated: 2025/06/14 19:15:12 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void error_unset(const char *identifier, t_shell *shell);

/* Helper to compare variable names */
static int is_match(const char *a, const char *b)
{
	if(!a || !b)
		return (0);
	return(strcmp(a, b) == 0);
}

/* Delete node with key from env_list */
static void	remove_env_key(const char *key, t_env **env_list)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (is_match(curr->key, key))
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}
int	is_valid_var_name(char *s)
{
	int	i;

	if (!s || !is_valid_var_char(s[0]) || s[0] == '\0'
		|| (s[0] >= '0' && s[0] <= '9'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!is_valid_var_char(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(char **args, t_env **env_list)
{
	int i;
	int has_error;

	i = 1;
	has_error = 0;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			error_unset(args[i], NULL);
			has_error = 1;
		}
		else
		{
			remove_env_key(args[i], env_list);
		}
		i++;
	}
	return has_error;
}
