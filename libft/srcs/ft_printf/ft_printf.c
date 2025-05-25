/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:33:40 by abby              #+#    #+#             */
/*   Updated: 2025/01/09 23:41:47 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
You have to recode the printf() function from libc.

Your function has to handle the following conversions: cspdiuxX%

Return Value：
printf() returns the number of characters printed after successful execution.
If an error occurs, a negative value is returned.
*/

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	count;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count += check_format(*format, args);
		}
		else
			count += handle_char(*format);
		format++;
	}
	va_end(args);
	return (count);
}
