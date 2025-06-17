/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:36:54 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 12:36:55 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Search for a node matching key; return it or NULL */
t_env	*find_node(const char *key, t_env *env)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

/* Create a new node with given key/value and push to head */
void	add_node(char *key, char *value, t_env **env)
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

/* Update or add a node to the environment */
void	update_or_add(char *key, char *value, t_env **env)
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
