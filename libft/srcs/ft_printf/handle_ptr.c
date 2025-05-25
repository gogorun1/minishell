/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:05:24 by wding             #+#    #+#             */
/*   Updated: 2025/01/10 00:17:22 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_ptr(unsigned long ptr)
{
	if (ptr == 0)
		return (handle_str("(nil)"));
	return (handle_str("0x") + handle_base(ptr, "0123456789abcdef", 16));
}
