/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:35:38 by lcao              #+#    #+#             */
/*   Updated: 2024/11/28 17:04:55 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*grand;
	char	*petit;
	size_t	i;
	size_t	j;

	grand = (char *)big;
	petit = (char *)little;
	i = 0;
	if (little[0] == '\0')
		return (grand);
	while (grand[i] && i < len)
	{
		j = 0;
		while (grand[i + j] == petit[j] && i + j < len && grand[i + j])
		{
			j++;
			if (petit[j] == '\0')
				return (grand + i);
		}
		i++;
	}
	return (NULL);
}
