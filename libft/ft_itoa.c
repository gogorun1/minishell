/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 17:32:01 by lcao              #+#    #+#             */
/*   Updated: 2024/12/03 17:08:35 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	count_len(int n)
{
	int	count;
	int	copy;

	count = 0;
	copy = n;
	if (n == 0)
		return (1);
	else if (n == -2147483648)
		return (11);
	else if (n < 0)
	{
		copy = -n;
		count += 1;
	}
	while (copy)
	{
		copy = copy / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		count;
	int		copy;

	count = count_len (n);
	copy = n;
	result = (char *)malloc(sizeof(char) * (count + 1));
	if (!result)
		return (NULL);
	if (n == 0)
		result[0] = '0';
	else if (n < 0)
		result[0] = '-';
	result[count] = '\0';
	while (copy)
	{
		if (copy < 0)
			result[count - 1] = -(copy % 10) + 48;
		else
			result[count - 1] = (copy % 10) + 48;
		copy = copy / 10;
		count--;
	}
	return (result);
}
