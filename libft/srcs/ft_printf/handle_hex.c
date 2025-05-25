/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:07:56 by wding             #+#    #+#             */
/*   Updated: 2025/01/10 00:16:32 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_hex(unsigned int arg, char id)
{
	if (id == 'x')
		return (handle_base(arg, "0123456789abcdef", 16));
	else
		return (handle_base(arg, "0123456789ABCDEF", 16));
}
