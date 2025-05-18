/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:08:48 by wding             #+#    #+#             */
/*   Updated: 2025/01/09 22:23:43 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_int(int arg)
{
	if (arg == -2147483648)
		return (handle_str("-2147483648"));
	else if (arg < 0 && arg > -2147483648)
		return (handle_char('-') + handle_unsigned(-arg));
	else
		return (handle_unsigned(arg));
}
