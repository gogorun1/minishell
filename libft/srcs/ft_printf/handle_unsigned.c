/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unsigned.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:50:22 by wding             #+#    #+#             */
/*   Updated: 2025/01/09 22:51:07 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_unsigned(unsigned int arg)
{
	return (handle_base(arg, "0123456789", 10));
}

/*
int main(void)
{
	int n = handle_unsigned(0);
	handle_char('\n');
	handle_unsigned(n);
}*/
