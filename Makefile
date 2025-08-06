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
	$(SRC_DIR)/builtins/builtins_opera1.c $(SRC_DIR)/builtins/get_env_list.c \
	$(SRC_DIR)/builtins/envlist_to_envp.c $(SRC_DIR)/builtins/builtins_opera2.c \
	$(SRC_DIR)/exec/exec_single.c $(SRC_DIR)/exec/exec_builtin.c $(SRC_DIR)/exec/exec_pipeline.c \
	$(SRC_DIR)/exec/get_path.c $(SRC_DIR)/exec/redirections.c \
	$(SRC_DIR)/parsing/lex.c $(SRC_DIR)/parsing/lex_utils.c $(SRC_DIR)/parsing/parser.c \
	$(SRC_DIR)/parsing/parser_utils.c $(SRC_DIR)/parsing/expander.c \
	$(SRC_DIR)/parsing/expander_utils.c \
	$(SRC_DIR)/utils/post_parsing_perror_exit.c \
	$(SRC_DIR)/utils/pre_parsing_error_exit.c \
	$(SRC_DIR)/utils/str_utils.c \
	$(SRC_DIR)/signals/signals.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	@$(MAKE) all -C $(LIBFT_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re
