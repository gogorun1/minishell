/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:34:14 by lcao              #+#    #+#             */
/*   Updated: 2024/12/03 17:53:50 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*p;
	int		i;

	i = ft_strlen(s);
	p = (char *)s;
	p += i;
	if (*p == (char)c)
		return (p);
	p--;
	i--;
	while (i >= 0)
	{
		if (*p == (char) c)
			return (p);
		p--;
		i--;
	}
	return (NULL);
}
