/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:15:58 by wding             #+#    #+#             */
/*   Updated: 2025/01/22 20:35:47 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	remainder = file_to_remainder(fd, remainder);
	if (!remainder)
		return (NULL);
	line = remainder_to_line(remainder);
	remainder = update_remainder(remainder);
	return (line);
}

char	*file_to_remainder(int fd, char *remainder)
{
	char	*buffer;
	ssize_t	byte_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	byte_read = 1;
	while (byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		if (byte_read == 0)
			break ;
		buffer[byte_read] = '\0';
		remainder = ft_strjoin_free(remainder, buffer);
		if (ft_strchr(remainder, '\n'))
			break ;
	}
	free(buffer);
	return (remainder);
}

char	*remainder_to_line(char *remainder)
{
	char	*line;
	size_t	lenth;

	if (!ft_strchr(remainder, '\n'))
	{
		line = ft_strdup(remainder);
		return (line);
	}
	lenth = (size_t)(ft_strchr(remainder, '\n') - remainder + 1);
	line = ft_substr(remainder, 0, lenth);
	return (line);
}

char	*update_remainder(char *remainder)
{
	size_t	start;
	size_t	lenth;
	char	*tmp;

	if (!ft_strchr(remainder, '\n'))
	{
		free(remainder);
		return (NULL);
	}
	lenth = ft_strlen(remainder);
	start = ft_strchr(remainder, '\n') - remainder + 1;
	if (!remainder[start])
	{
		free(remainder);
		return (NULL);
	}
	tmp = ft_substr(remainder, start, lenth - start);
	free(remainder);
	return (tmp);
}

char	*ft_strjoin_free(char *remainder, char *buffer)
{
	char	*join;
	size_t	lenth;

	if (!remainder)
		remainder = ft_strdup("");
	lenth = ft_strlen(remainder) + ft_strlen(buffer);
	join = (char *)malloc(lenth + 1);
	if (!join)
		return (NULL);
	ft_strcpy(join, remainder);
	ft_strcpy(join + ft_strlen(remainder), buffer);
	join[lenth] = '\0';
	free(remainder);
	return (join);
}
