/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:52:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/24 19:16:40 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_option_n(const char *str)
{
	int i;

	if(!str || str[0] != '-')
		return (0);
	i = 1;
	while(str[i])
	{
		if(str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

int builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	while(args[i] && is_option_n(args[i]))
	{
		newline = 0;
		i++;
	}
	while(args[i])
	{
		printf("%s", args[i]);
		if(args[i + 1])
			printf(" ");
		i++;
	}
	if(newline)
		printf("\n");
	return (0);
}