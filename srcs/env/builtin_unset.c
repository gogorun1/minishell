/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:04:03 by lcao              #+#    #+#             */
/*   Updated: 2025/05/24 15:13:14 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_env	*curr = *env_list;
	t_env	*prev = NULL;

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

int	builtin_unset(char **args, t_env **env_list)
{
	int	i = 1;

	while (args[i])
	{
		remove_env_key(args[i], env_list);
		i++;
	}
	return (0);
}

