NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror

# Paths
SRC_DIR := src
OBJ_DIR := obj
LIBFT_PATH := libft
LIBFT := $(LIBFT_PATH)/libft.a
INCLUDE := -I include

# Sources
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/builtins/builtins.c \
	$(wildcard $(SRC_DIR)/exec/*.c) \
	$(wildcard $(SRC_DIR)/parsing/*.c) \
	$(wildcard $(SRC_DIR)/utils/*.c) \
	$(SRC_DIR)/repl/repl.c \
	$(SRC_DIR)/signals/signals.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	$(MAKE) all -C $(LIBFT_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re