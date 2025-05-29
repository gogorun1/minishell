/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:27:54 by lcao              #+#    #+#             */
/*   Updated: 2025/05/28 18:28:01 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Write string to fd
static int	ft_putstr_fd_len(char *s, int fd)
{
	int	len;

	if (!s)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	len = strlen(s);
	write(fd, s, len);
	return (len);
}

// Write number to fd
static int	ft_putnbr_fd_len(int n, int fd)
{
	char	*str;
	int		len;

	str = ft_itoa(n);
	if (!str)
		return (0);
	len = strlen(str);
	write(fd, str, len);
	free(str);
	return (len);
}

// Write char to fd
static int	ft_putchar_fd_len(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

// Process format specifier
static int	process_format(const char **format, va_list args, int fd)
{
	char	specifier;

	(*format)++;
	specifier = **format;
	if (specifier == 's')
		return (ft_putstr_fd_len(va_arg(args, char *), fd));
	else if (specifier == 'd' || specifier == 'i')
		return (ft_putnbr_fd_len(va_arg(args, int), fd));
	else if (specifier == 'c')
		return (ft_putchar_fd_len(va_arg(args, int), fd));
	else if (specifier == '%')
		return (ft_putchar_fd_len('%', fd));
	else
	{
		ft_putchar_fd_len('%', fd);
		ft_putchar_fd_len(specifier, fd);
		return (2);
	}
}

// Custom fprintf implementation
int	ft_fprintf(int fd, const char *format, ...)
{
	va_list		args;
	int			count;
	const char	*ptr;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	ptr = format;
	while (*ptr)
	{
		if (*ptr == '%' && *(ptr + 1))
		{
			count += process_format(&ptr, args, fd);
			ptr++;
		}
		else
		{
			count += ft_putchar_fd_len(*ptr, fd);
			ptr++;
		}
	}
	va_end(args);
	return (count);
}
