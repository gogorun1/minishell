/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:48:48 by wding             #+#    #+#             */
/*   Updated: 2024/11/26 23:14:16 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
trim the *s1 by the characters in *set from the begining and in the end
*/

static int	isset(char const *set, const char c)
{
	while (set && *set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (isset(set, s1[start]))
		start++;
	len = ft_strlen(s1);
	while (len > 0 && isset(set, s1[len - 1]) && len > start)
		len--;
	return (ft_substr(s1, start, len - start));
}

/*#include <stdio.h>
#include "libft.h"

int main() 
{
    char *result;

	result = ft_strtrim("   Hello, World!   ", " ");
    printf("Expected output: 'Hello, World!', '%s'\n", result);
    free(result);

    result = ft_strtrim("xxxyHelloxy", "xy");
    printf("Expected output: 'Hello', '%s'\n", result);
    free(result);

    result = ft_strtrim("abcabc", "abc");
    printf("Expected output: '', '%s'\n", result);
    free(result);
	
    result = ft_strtrim("", "abc");
    printf("Expected output: '', '%s'\n", result);
    free(result);	

	result = ft_strtrim(NULL, "abc");
    printf("Expected output: NULL, '%s'\n", result);
    free(result);

	result = ft_strtrim("abc", NULL);
    printf("Expected output: NULL, '%s'\n", result);
    free(result);

    return 0;
}*/