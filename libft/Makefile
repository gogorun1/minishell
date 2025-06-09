# define the variables
NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes

# directories
INCLUDES = includes
SRCS_DIR = srcs
OBJ_DIR = obj

# source files
LIBFT_SRCS = $(SRCS_DIR)/libft/ft_atoi.c $(SRCS_DIR)/libft/ft_itoa.c $(SRCS_DIR)/libft/ft_tolower.c $(SRCS_DIR)/libft/ft_toupper.c \
		$(SRCS_DIR)/libft/ft_isalpha.c $(SRCS_DIR)/libft/ft_isalnum.c $(SRCS_DIR)/libft/ft_isascii.c $(SRCS_DIR)/libft/ft_isdigit.c $(SRCS_DIR)/libft/ft_isprint.c \
		$(SRCS_DIR)/libft/ft_memchr.c $(SRCS_DIR)/libft/ft_memcmp.c $(SRCS_DIR)/libft/ft_memcpy.c $(SRCS_DIR)/libft/ft_memmove.c $(SRCS_DIR)/libft/ft_memset.c $(SRCS_DIR)/libft/ft_bzero.c $(SRCS_DIR)/libft/ft_calloc.c \
		$(SRCS_DIR)/libft/ft_striteri.c $(SRCS_DIR)/libft/ft_strmapi.c $(SRCS_DIR)/libft/ft_putchar_fd.c $(SRCS_DIR)/libft/ft_putendl_fd.c $(SRCS_DIR)/libft/ft_putnbr_fd.c $(SRCS_DIR)/libft/ft_putstr_fd.c \
		$(SRCS_DIR)/libft/ft_split.c $(SRCS_DIR)/libft/ft_strchr.c $(SRCS_DIR)/libft/ft_strrchr.c $(SRCS_DIR)/libft/ft_strdup.c $(SRCS_DIR)/libft/ft_strjoin.c $(SRCS_DIR)/libft/ft_strlcat.c \
		$(SRCS_DIR)/libft/ft_strlcpy.c $(SRCS_DIR)/libft/ft_strlen.c $(SRCS_DIR)/libft/ft_strncmp.c $(SRCS_DIR)/libft/ft_strnstr.c $(SRCS_DIR)/libft/ft_strtrim.c $(SRCS_DIR)/libft/ft_substr.c \
		$(SRCS_DIR)/libft/ft_lstnew_bonus.c $(SRCS_DIR)/libft/ft_lstadd_front_bonus.c $(SRCS_DIR)/libft/ft_lstsize_bonus.c \
        $(SRCS_DIR)/libft/ft_lstlast_bonus.c $(SRCS_DIR)/libft/ft_lstadd_back_bonus.c $(SRCS_DIR)/libft/ft_lstdelone_bonus.c \
        $(SRCS_DIR)/libft/ft_lstclear_bonus.c $(SRCS_DIR)/libft/ft_lstiter_bonus.c $(SRCS_DIR)/libft/ft_lstmap_bonus.c \
		$(SRCS_DIR)/libft/ft_strcpy.c

PRINTF_SRCS = $(SRCS_DIR)/ft_printf/ft_printf.c \
		$(SRCS_DIR)/ft_printf/check_format.c \
		$(SRCS_DIR)/ft_printf/handle_char.c \
		$(SRCS_DIR)/ft_printf/handle_base.c \
		$(SRCS_DIR)/ft_printf/handle_hex.c \
		$(SRCS_DIR)/ft_printf/handle_int.c \
		$(SRCS_DIR)/ft_printf/handle_ptr.c \
		$(SRCS_DIR)/ft_printf/handle_str.c \
		$(SRCS_DIR)/ft_printf/handle_unsigned.c

GNL_SRCS = $(SRCS_DIR)/get_next_line/get_next_line.c \
		$(SRCS_DIR)/get_next_line/get_next_line_bonus.c

# Object files
LIBFT_OBJS = $(LIBFT_SRCS:$(SRCS_DIR)/libft/%.c=$(OBJ_DIR)/libft/%.o)
PRINTF_OBJS = $(PRINTF_SRCS:$(SRCS_DIR)/ft_printf/%.c=$(OBJ_DIR)/ft_printf/%.o)
GNL_OBJS = $(GNL_SRCS:$(SRCS_DIR)/get_next_line/%.c=$(OBJ_DIR)/get_next_line/%.o)

OBJS = $(LIBFT_OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

# format
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_BOLD = \033[1m
COLOR_RESET = \033[0m

# rules
all: make_dirs $(NAME)

make_dirs:
	@mkdir -p $(OBJ_DIR)/libft
	@mkdir -p $(OBJ_DIR)/ft_printf
	@mkdir -p $(OBJ_DIR)/get_next_line

# Compile libft objects
$(OBJ_DIR)/libft/%.o: $(SRCS_DIR)/libft/%.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# Compile printf objects
$(OBJ_DIR)/ft_printf/%.o: $(SRCS_DIR)/ft_printf/%.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# Compile gnl objects
$(OBJ_DIR)/get_next_line/%.o: $(SRCS_DIR)/get_next_line/%.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@ 

# create the library
$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Libft is created $(COLOR_RESET)🚀🚀"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(COLOR_BLUE)$(COLOR_BOLD)Libft object files removed$(COLOR_RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(COLOR_BLUE)$(COLOR_BOLD)Libft removed$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re make_dirs