/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:24:16 by lcao              #+#    #+#             */
/*   Updated: 2025/03/03 17:53:57 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

static	size_t	count_word(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && !(s[i] == c))
		i++;
	return (i);
}

static	size_t	count_array(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == c))
			i++;
		if (s[i])
			count++;
		while (s[i] && !(s[i] == c))
			i++;
	}
	return (count);
}

static	char	*get_word(char const *s, char c)
{
	char	*word;
	size_t	nb;

	nb = count_word(s, c);
	word = (char *)malloc(sizeof(char) * (nb + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, s, nb + 1);
	return (word);
}

char	**free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return (NULL);
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tal;
	size_t	nb;
	size_t	j;

	j = 0;
	nb = count_array(s, c);
	tal = (char **)malloc(sizeof(char *) * (nb + 1));
	if (!tal)
		return (NULL);
	while (*s)
	{
		while (*s && (*s == c))
			s++;
		if (*s)
		{
			nb = count_word(s, c);
			tal[j] = get_word(s, c);
			if (!(tal[j]))
				return (free_split(tal), NULL);
			s += nb;
			j++;
		}
	}
	tal[j] = NULL;
	return (tal);
}
