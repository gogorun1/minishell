/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-25 19:00:05 by wding             #+#    #+#             */
/*   Updated: 2024-11-25 19:00:05 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a string
representing the integer received as an argument.
Negative numbers must be handled.
*/

long	len_nbr(long n)
{
	long	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*t_num(long n)
{
	char	*num;
	long	rest;
	long	len;

	len = len_nbr(n);
	num = malloc(len + 1);
	if (num == NULL)
		return (NULL);
	num[len] = '\0';
	while (len--)
	{
		rest = n % 10;
		num[len] = rest + 48;
		n = n / 10;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	char	*num;
	char	*num0;
	long	t_n;

	t_n = n;
	if (n < 0)
	{
		t_n = -t_n;
		num0 = t_num(t_n);
		if (num0 == NULL)
			return (NULL);
		num = ft_strjoin("-", num0);
		free(num0);
	}
	else
		num = t_num(t_n);
	return (num);
}

/*
#include <stdio.h>

int	main()
{
	int	n = 213429402;

	printf("result is %s\n", ft_itoa(n));
	printf("result is %s\n", ft_itoa(-n));
	return(0);
}
*/