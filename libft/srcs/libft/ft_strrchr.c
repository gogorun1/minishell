/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:46:44 by wding             #+#    #+#             */
/*   Updated: 2024/11/15 01:04:47 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**The strrchr() function returns a pointer to the last occurrence of the
**character c in the string s.
**if c is '\0', the functions locate
**the terminating '\0'.
*/

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s) + 1;
	while (len--)
	{
		if (s[len] == (char)c)
			return ((char *)(s + len));
	}
	return (NULL);
}
