/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:18:02 by lcao              #+#    #+#             */
/*   Updated: 2025/05/15 17:30:45 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **split_input(char *input)
{
	char	**args;
	char	*token;
	int		i;
	
	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	i = 0;
	token = strtok(input, " ");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}
