/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 20:51:27 by wding             #+#    #+#             */
/*   Updated: 2024-11-20 20:51:27 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a substring
from the string ’s’.
The substring begins at index ’start’ and is of
maximum size ’len
*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	len_s;
	size_t	i;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start >= len_s)
		len = 0;
	else if (len > len_s - start)
		len = len_s - start;
	sub = malloc(sizeof(char) * len + 1);
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

/*
#include <stdio.h>

int main()
{
	printf("Case 1: Substring within bounds\n expected def, result is %s\n", 
	ft_substr("abcdefg", 3, 3));
	printf("Case 2: Start beyond string length\n expected \"\", result is %s\n", 
	ft_substr("abcdefg", 10, 3));
	printf("Case 3: Length exceeds available characters\nexpected \"defg\", 
	result is %s\n", ft_substr("abcdefg", 3, 10));
	printf("Case 4: Full string\nexpected \"abcdefg\", result is %s\n", 
	ft_substr("abcdefg", 0, 7));
	printf("Case 5: Empty input string\n expected \"\", result is %s\n", 
	ft_substr("", 0, 3));
	printf("Case 6: Null input\n expected null, result is %s\n", 
	ft_substr(NULL, 3, 10));

	return (0);
}
*/