NAME = minishell
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRCS = \
		main.c \
		src/split_input.c \
		src/builtin/builtin.c \
		src/builtin/builtin_cd.c \
		src/builtin/builtin_exit.c \
		src/builtin/builtin_pwd.c \
		src/builtin/builtin_export.c \
		src/builtin/builtin_echo.c \
		src/env/builtin_env_list.c \
		src/env/builtin_unset.c \
		src/env/init_env.c \
		src/utils/ft_strndup.c

OBJS = $(SRCS:.c=.o)

CFLAGS  = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lreadline

CC = cc
RM = rm -f

all:$(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re $(LIBFT)