/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-21 16:02:53 by wding             #+#    #+#             */
/*   Updated: 2024-11-21 16:02:53 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a new
string, which is the result of the concatenation
of ’s1’ and ’s2’.
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*r;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	r = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!r)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		r[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		r[i + j] = s2[j];
		j++;
	}
	r[i + j] = '\0';
	return (r);
}

/*#include <stdio.h>

int main()
{
	printf("1: normal\n expected abcdefgadd, result is %s\n", 
	ft_strjoin("abcdefg", "add"));
	printf("2: input null\n expected null, result is %s\n", 
	ft_strjoin(NULL, NULL));
	printf("3: 1st input is null\nexpected \"abcdefg\", result is %s\n", 
	ft_strjoin(NULL, "abcdefg"));
	printf("4: 2nd input is null\nexpected \"abcdefg\", result is %s\n", 
	ft_strjoin("abcdefg", NULL));
	printf("5: Empty input string\n expected \"\", result is %s\n", 
	ft_strjoin("", ""));
	char *s1 = "my favorite animal is ";
        char *s2 = s1 + 20;
        char *res = ft_strjoin(s2, s1);
	printf("6: Overlap strings\n expected \"s my favorite animal is \", 
	result is \"%s\"\n", res);
	return (0);
}*/