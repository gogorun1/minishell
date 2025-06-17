/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:33:48 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 11:49:38 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env_list(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (cur->key && cur->value)
			printf("%s=%s\n", cur->key, cur->value);
		else if (cur->key)
			printf("%s=\n", cur->key);
		cur = cur->next;
	}
	return (0);
}

int	builtin_env_list(t_env *env, t_shell *shell, char **args)
{
	if (!env)
	{
		ft_fprintf(2, "minishell: env: no environment variables set\n");
		if (shell)
			shell->last_exit_status = 1;
		return (1);
	}
	if (args && args[1])
	{
		ft_fprintf(2, "minishell: env: %s: No such file or directory\n",
			args[1]);
		if (shell)
			shell->last_exit_status = 127;
		return (127);
	}
	if (shell)
		shell->last_exit_status = 0;
	return (print_env_list(env));
}
