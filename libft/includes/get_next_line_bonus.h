/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:17:25 by wding             #+#    #+#             */
/*   Updated: 2025/01/22 20:40:23 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef FD_MAX
#  define FD_MAX 10240
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);
char	*file_to_remainder(int fd, char *remainder);
char	*remainder_to_line(char *remainder);
char	*update_remainder(char *remainder);
char	*ft_strjoin_free(char *remainder, char *buffer);

#endif