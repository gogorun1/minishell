/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 01:55:36 by wding             #+#    #+#             */
/*   Updated: 2024/11/15 00:03:19 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
checks  for  an  alphabetic  character;  in  the  standard  "C" locale, 
it is equivalent to (isupper(c) || islower(c)).  
In some locales, there may be additional characters for which 
isalpha() is true—letters which are neither uppercase nor lowercase.
*/

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90)
		|| (c >= 97 && c <= 122))
		return (1);
	return (0);
}
