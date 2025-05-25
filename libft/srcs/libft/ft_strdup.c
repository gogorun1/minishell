/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-15 17:22:12 by wding             #+#    #+#             */
/*   Updated: 2024-11-15 17:22:12 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//duplicate a string to another address

char	*ft_strdup(const char *s)
{
	char	*d;
	char	*ptr;
	size_t	len;

	len = ft_strlen(s);
	d = malloc(len + 1);
	if (d == NULL)
		return (NULL);
	ptr = d;
	while (len)
	{
		*d++ = *s++;
		len--;
	}
	*d = '\0';
	return (ptr);
}
