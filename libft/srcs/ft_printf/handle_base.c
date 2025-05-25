/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_base.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:46:33 by wding             #+#    #+#             */
/*   Updated: 2025/01/09 22:51:51 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_base(unsigned long long arg, char *base, int base_len)
{
	int	count;

	count = 0;
	if (arg >= (unsigned long long)base_len)
		count += handle_base(arg / base_len, base, base_len);
	count += handle_char(base[arg % base_len]);
	return (count);
}
