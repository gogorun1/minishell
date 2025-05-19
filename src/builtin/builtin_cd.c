/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:43:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/19 16:29:45 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args)
{
	const char	*path;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 18);
			return (1);
		}
	}
	else
		path = args[1];
	printf("🧭 [cd] trying to go to: \"%s\"\n", path);
		
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

