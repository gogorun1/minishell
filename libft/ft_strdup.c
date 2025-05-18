/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:46:13 by lcao              #+#    #+#             */
/*   Updated: 2024/12/02 19:54:24 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*new;
	size_t	len_s;

	len_s = ft_strlen(s);
	new = (char *)malloc(sizeof(char) * (len_s + 1));
	if (!new)
		return (NULL);
	ft_strlcpy(new, s, len_s + 1);
	return (new);
}
