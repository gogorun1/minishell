/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-25 22:50:33 by wding             #+#    #+#             */
/*   Updated: 2024-11-25 22:50:33 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Outputs the integer ’n’ to the given file
descriptor.
*/

void	ft_putnbr_fd(int n, int fd)
{
	long	i;
	long	j;
	char	c;

	i = n;
	if (n < 0)
	{
		i = -i;
		ft_putchar_fd('-', fd);
	}
	j = i % 10;
	i = i / 10;
	c = j + '0';
	if (i > 0)
		ft_putnbr_fd(i, fd);
	ft_putchar_fd(c, fd);
}
