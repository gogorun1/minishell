/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:27:42 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 14:52:26 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_export_arg_with_equal(char *arg, char *eq, t_env **env,
		t_shell *shell)
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
	value = ft_strdup(eq + 1);
	update_or_add(key, value, env);
	free(value);
	return (0);
}

static void	print_all_env(t_env *env)
{
	while (env)
	{
		printf("export %s", env->key);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

static int	export_no_args(t_env **env)
{
	print_all_env(*env);
	return (0);
}

static int	export_with_args(char **args, t_env **env, t_shell *shell)
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

int	builtin_export(char **args, t_env **env, t_shell *shell)
{
	if (!args[1])
		return (export_no_args(env));
	else
		return (export_with_args(args, env, shell));
}
