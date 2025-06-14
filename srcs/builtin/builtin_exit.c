/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:11:39 by lcao              #+#    #+#             */
/*   Updated: 2025/06/14 17:56:48 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_number(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	status;

	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!is_number(args[1]))
	{
		ft_fprintf(2, "minishell: exit: %s: numeric argument required\n",
			args[1]);
		exit(2);
	}
	if (args[2])
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	status = ft_atoi(args[1]);
	exit((unsigned char)status);
}
