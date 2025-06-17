/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:06:06 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 23:13:41 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Concatenate two strings. 
** Appends src to string dst of size siz (unlike strncat(), 
** siz is the full size of dst, not space left). 
** At most siz-1 characters will be copied. Always NULL terminates 
** (unless siz <= ft_strlen(dst)). */

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	ld;
	size_t	ls;
	size_t	i;

	i = 0;
	ld = ft_strlen(dst);
	ls = ft_strlen(src);
	if (siz <= ld || siz == 0)
		return (ls + siz);
	while (src[i] && ((ld + i + 1) < siz))
	{
		dst[ld + i] = src[i];
		i++;
	}
	dst[ld + i] = '\0';
	return (ls + ld);
}
