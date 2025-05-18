/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:11:39 by lcao              #+#    #+#             */
/*   Updated: 2025/05/18 17:24:21 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int	is_number(const char *str)
{
	int	i;

	i = 0;
	if(!str)
		return (0);
	if(str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}*/
int	builtin_exit(char **args)
{
	(void)args;
	write(1, "exit\n", 5);
	exit(0);
}
