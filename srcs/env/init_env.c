/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:31:34 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 10:48:51 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_env_node(char *str)
{
	t_env	*new;
	char	*equal;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	equal = ft_strchr(str, '=');
	if (equal)
	{
		new->key = ft_strndup(str, equal - str);
		new->value = ft_strdup(equal + 1);
	}
	else
	{
		new->key = ft_strdup(str);
		new->value = NULL;
	}
	new->next = NULL;
	return (new);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new = create_env_node(envp[i]);
		if (!new)
			return (NULL);
		new->next = head;
		head = new;
		i++;
	}
	return (head);
}
