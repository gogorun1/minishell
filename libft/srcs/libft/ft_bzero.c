/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:24:48 by wding             #+#    #+#             */
/*   Updated: 2024/11/15 00:04:52 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
The  bzero() function erases the data in the n bytes of the memory 
starting at the location pointed to by s, by writing zeros 
(bytes containing '\0') to that area.
*/

void	ft_bzero(void *s, size_t n)
{
	while (n--)
	{
		*(char *)s = 0;
		s++;
	}
}
