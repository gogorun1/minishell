/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:04:31 by lcao              #+#    #+#             */
/*   Updated: 2025/05/30 19:43:38 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_env_vars(t_env *env);
char	**env_to_array(t_env *env);
void	free_env_array(char **envp);
void	free_env_array_partial(char **envp, int count);
//count_env_vars: 计算环境变量数
//env_to_array: 将t_env链表转换为execve需要的字符串数组格式
//free_env_array: 释放整个环境变量数组
//free_env_array_partial: 部分释放数组（错误处理时使用）

// Count number of environment variables
int	count_env_vars(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// Create environment string from key and value
/*char	*create_env_string(char *key, char *value)
{
	char	*result;
	int		key_len;
	int		value_len;
	int		i;

	if (!key || !value)
		return (NULL);
	key_len = strlen(key);
	value_len = strlen(value);
	result = malloc(key_len + value_len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (i < key_len)
	{
		result[i] = key[i];
		i++;
	}
	result[i++] = '=';
	while (i - key_len - 1 < value_len)
	{
		result[i] = value[i - key_len - 1];
		i++;
	}
	result[i] = '\0';
	return (result);
}
*/

// Convert environment linked list to array
char	**env_to_array(t_env *env)
{
	char	**envp;
	int		count;
	int		i;
	t_env	*current;

	count = count_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current && i < count)
	{
		envp[i] = create_env_string(current->key, current->value);
		if (!envp[i])
		{
			free_env_array_partial(envp, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// Free environment array completely
void	free_env_array(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

// Free environment array partially (for error handling)
void	free_env_array_partial(char **envp, int count)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (i < count)
	{
		if (envp[i])
			free(envp[i]);
		i++;
	}
	free(envp);
}
