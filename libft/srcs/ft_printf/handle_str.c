/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 12:57:41 by wding             #+#    #+#             */
/*   Updated: 2025/01/10 00:06:03 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_str(char *arg)
{
	int	count;

	count = 0;
	if (arg == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (*arg)
	{
		count += handle_char(*arg);
		arg++;
	}
	return (count);
}
