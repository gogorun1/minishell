/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:52:40 by wding             #+#    #+#             */
/*   Updated: 2024/11/27 01:01:15 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates and reurns an array of strings obtained by splitting 's'
using the character 'c' as a delimiter. The array must end with a
NULL pointer
*/

static size_t	count_w(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (s && *s)
	{
		if (*s != c)
		{
			count++;
			s++;
		}
		while (*s && *s != c)
			s++;
		while (*s && *s == c)
			s++;
	}
	return (count);
}

static void	*free_all(char **strset, size_t count)
{
	size_t	i;

	i = 0;
	while (i <= count)
	{
		free(strset[i]);
		i++;
	}
	free(strset);
	return (NULL);
}

static size_t	get_next_word(const char *s, char c, size_t *start)
{
	size_t	len;

	len = 0;
	while (s[*start] == c)
		(*start)++;
	while (s[*start + len] && s[*start + len] != c)
		len++;
	return (len);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	len;
	size_t	count;
	char	**sp;

	count = count_w(s, c);
	sp = (char **)malloc(sizeof (char *) * (count + 1));
	if (sp == NULL)
		return (NULL);
	i = 0;
	start = 0;
	while (i < count)
	{
		len = get_next_word(s, c, &start);
		sp[i] = ft_substr(s, start, len);
		if (!sp[i])
			return (free_all(sp, i));
		start += len;
		i++;
	}
	sp[i] = NULL;
	return (sp);
}

/*
#include <stdio.h>

void	print(char *str[])
{
	size_t i;

	i = 0;
	while (str[i] != NULL)
	{
		while (*str[i])
		{
			printf("%c", *str[i]);
			str[i]++;			
		}
		i++;
	}
	printf("\n");
}

int	main(void)
{
	char const	*str1 = "xxHelloxxWorldx!";
	char const	*str2 = "HelloxxWorldx!";
	char const	*str3 = "xxx";
	char const	*str4 = "Hello";
	char	c = 'x';
	print(ft_split(str1, c));
	print(ft_split(str2, c));
	print(ft_split(str3, c));
	print(ft_split(str4, c));
}
*/