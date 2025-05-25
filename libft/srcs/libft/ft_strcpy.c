/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abby <abby@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:47:54 by wding             #+#    #+#             */
/*   Updated: 2025/03/27 22:23:54 by abby             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, char *src)
{
	char	*dst_start;

	dst_start = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (dst_start);
}
