NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g
LIBS = -lreadline
RM = rm -f

SRCS_DIR = srcs/
OBJS_DIR = objs/
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

INCLUDES = -Iincludes -Ilibft/includes

SRCS = mainprint.c \
		main.c \
		split_input.c \
		builtin/builtin.c \
		builtin/builtin_cd.c \
		builtin/builtin_exit.c \
		builtin/builtin_pwd.c \
		builtin/builtin_export.c \
		builtin/builtin_echo.c \
		env/builtin_env_list.c \
		env/builtin_unset.c \
		env/init_env.c \
		tokenisation/tokenizer.c \
		parsing/parser.c \
		utils/ft_strndup.c \
		utils/utils.c \
		temps/print.c \

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

# format
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_BOLD = \033[1m
COLOR_RESET = \033[0m

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)/builtin \
				$(OBJS_DIR)/env \
				$(OBJS_DIR)/utils \
				$(OBJS_DIR)/parsing \
				$(OBJS_DIR)/tokenisation \
				$(OBJS_DIR)/temps \

	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft $(LIBS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)$(NAME) is created $(COLOR_RESET)"

clean:
	@$(RM) -r $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(COLOR_BLUE)$(COLOR_BOLD)Objects removed$(COLOR_RESET)"

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(COLOR_BLUE)$(COLOR_BOLD)all clear$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re