/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:03:25 by wding             #+#    #+#             */
/*   Updated: 2025/01/10 00:09:36 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_format(const char format, va_list args)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += handle_char(va_arg(args, int));
	else if (format == 's')
		count += handle_str(va_arg(args, char *));
	else if (format == 'p')
		count += handle_ptr(va_arg(args, unsigned long));
	else if (format == 'd' || format == 'i')
		count += handle_int(va_arg(args, int));
	else if (format == 'u')
		count += handle_unsigned(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		count += handle_hex(va_arg(args, unsigned long int), format);
	else if (format == '%')
		count += handle_char('%');
	return (count);
}
