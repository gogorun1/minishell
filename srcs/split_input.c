/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:18:02 by lcao              #+#    #+#             */
/*   Updated: 2025/05/19 16:10:33 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_spaces(char *s, int i)
{
	while (s[i] && s[i] == ' ')
		i++;
	return (i);
}

static int	skip_word(char *s, int i)
{
	while (s[i] && s[i] != ' ')
		i++;
	return (i);
}

char	**split_input(char *input)
{
	char	**args;
	int		i = 0;
	int		j = 0;
	int		start;

	args = malloc(sizeof(char *) * 100);
	if (!args)
		return (NULL);
	while (input[i])
	{
		i = skip_spaces(input, i);
		start = i;
		i = skip_word(input, i);
		if (i > start)
			args[j++] = ft_strndup(&input[start], i - start);
	}
	args[j] = NULL;
	return (args);
}

