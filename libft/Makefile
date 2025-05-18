
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 15:28:30 by lcao              #+#    #+#              #
#    Updated: 2024/12/02 21:05:46 by lcao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = ft_isalpha.c ft_putnbr_fd.c ft_isdigit.c ft_split.c \
	ft_isprint.c ft_strjoin.c ft_strtrim.c ft_memset.c \
	ft_strdup.c ft_tolower.c ft_strlcat.c ft_strchr.c \
	ft_calloc.c ft_memcpy.c ft_itoa.c \
	ft_striteri.c ft_substr.c ft_strnstr.c ft_strlen.c \
	ft_bzero.c ft_isascii.c ft_isalnum.c ft_strncmp.c \
	ft_putchar_fd.c ft_strmapi.c ft_putendl_fd.c ft_strrchr.c \
	ft_memmove.c ft_strlcpy.c ft_memcmp.c ft_toupper.c \
	ft_putstr_fd.c ft_memchr.c ft_atoi.c

NAME	:= libft.a
		
OBJS   := $(SRCS:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	ar rc $@ $(OBJS)

%.o : %.c
	cc -Wall -Werror -Wextra -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
