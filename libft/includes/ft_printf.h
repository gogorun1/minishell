/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:11:06 by abby              #+#    #+#             */
/*   Updated: 2025/01/10 00:17:40 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	handle_char(char c);
int	handle_str(char *str);
int	handle_ptr(unsigned long ptr);
int	handle_base(unsigned long long arg, char *base, int base_len);
int	handle_hex(unsigned int arg, char id);
int	handle_int(int arg);
int	handle_unsigned(unsigned int arg);
int	check_format(const char format, va_list args);
int	ft_printf(const char *format, ...);

#endif