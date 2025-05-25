/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:19:20 by wding             #+#    #+#             */
/*   Updated: 2024/11/13 10:25:19 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Whether c is a 7-bit unsigned char value that fits into the ASCII character set
*/

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
