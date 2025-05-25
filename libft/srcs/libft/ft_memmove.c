/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:02:31 by wding             #+#    #+#             */
/*   Updated: 2024/11/15 00:03:46 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	if (dest > src && dest < src + n)
	{
		d = dest + n;
		s = src + n;
		while (n--)
			*--d = *--s;
	}
	else
	{
		d = dest;
		s = src;
		while (n--)
			*d++ = *s++;
	}
	return ((void *)dest);
}
