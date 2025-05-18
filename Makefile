NAME = minishell

SRCS = \
		main.c \
		src/split_input.c \
		src/builtin/builtin.c \
		src/builtin/builtin_cd.c \
		src/builtin/builtin_exit.c \
		src/builtin/builtin_pwd.c \
		src/builtin/builtin_env.c 

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -Iincludes

CC = cc
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

clean:
	$(RM) $(OBJS)

re: fclean all