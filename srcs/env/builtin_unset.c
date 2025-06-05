/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:04:03 by lcao              #+#    #+#             */
/*   Updated: 2025/06/05 16:08:27 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 声明外部实现的变量名检查和错误输出函数
bool is_valid_var_name(const char *s);
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
