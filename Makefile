NAME = minishell

SRCS = \
		main.c \
		src/split_input.c \
		src/builtin.c \
		src/builtin_cd.c \
		src/builtin_exit.c \
		src/builtin_pwd.c 

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