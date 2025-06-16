/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:06:10 by lcao              #+#    #+#             */
/*   Updated: 2025/06/16 19:11:19 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_env_string(char *dest, char *key, char *value);
char		*create_env_string(char *key, char *value);
void		free_env(t_env *head);

static void	fill_env_string(char *dest, char *key, char *value)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (key[i])
	{
		dest[i] = key[i];
		i++;
	}
	dest[i++] = '=';
	while (value[j])
	{
		dest[i++] = value[j++];
	}
	dest[i] = '\0';
}

//create_env_string: 创建"KEY=VALUE"格式的环境变量字符串
//Create an environment variable string in the format 'KEY=VALUE'”
char	*create_env_string(char *key, char *value)
{
	char	*result;
	int		key_len;
	int		value_len;

	if (!key || !value)
		return (NULL);
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	result = malloc(key_len + value_len + 2);
	if (!result)
		return (NULL);
	fill_env_string(result, key, value);
	return (result);
}

void	free_env(t_env *head)
{
	t_env	*tmp;
	while(head)
	{
		tmp = head;
		head = head->next;
		// printf("free env: %s=%s\n", tmp->key, tmp->value);
		if (tmp->key)
			free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
