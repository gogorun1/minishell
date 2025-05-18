/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-15 15:13:39 by wding             #+#    #+#             */
/*   Updated: 2024-11-15 15:13:39 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
The memcmp() function compares the first n bytes 
(each interpreted as unsigned char) of the memory areas s1 and s2.
*/

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*t1;
	const unsigned char	*t2;

	t1 = s1;
	t2 = s2;
	while (n)
	{
		if (*t1 != *t2)
			break ;
		t1++;
		t2++;
		n--;
	}
	if (n > 0)
		return ((int)(*t1 - *t2));
	return (0);
}
