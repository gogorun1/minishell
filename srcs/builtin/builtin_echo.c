/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:52:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/05 16:44:08 by lcao             ###   ########.fr       */
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

static void print_arg(const char *arg)
{
    int i;
    int len;

    if (!arg)
        return;
    len = (int)ft_strlen(arg);
    if (len >= 2 && ((arg[0] == '\"' && arg[len - 1] == '\"') ||
        (arg[0] == '\'' && arg[len - 1] == '\'')) && len > 2)
    {
        i = 1;
        while (i < len - 1)
        {
            putchar(arg[i]);
            i++;
        }
    }
    else if (len == 2 && ((arg[0] == '\"' && arg[1] == '\"') || (arg[0] == '\'' && arg[1] == '\'')))
    {
    }
    else
    {
        printf("%s", arg);
    }
}

int builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	while (args[i] && is_option_n(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		print_arg(args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}