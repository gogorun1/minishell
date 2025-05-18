/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 15:45:58 by lcao              #+#    #+#             */
/*   Updated: 2024/12/02 19:23:42 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	check_char(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*copy;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && check_char(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && check_char(s1[end - 1], set))
		end--;
	len = end - start;
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	ft_strlcpy(copy, &s1[start], len + 1);
	return (copy);
}
