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
	$(SRC_DIR)/builtins/builtins_opera3.c $(SRC_DIR)/builtins/builtins_utils.c \
	$(SRC_DIR)/exec/exec_single.c $(SRC_DIR)/exec/exec_builtin.c \
	$(SRC_DIR)/exec/heredoc_pipeline_utils.c $(SRC_DIR)/exec/heredoc.c \
	$(SRC_DIR)/exec/exec_pipeline.c $(SRC_DIR)/exec/get_path.c $(SRC_DIR)/exec/redirections.c \
	$(SRC_DIR)/parsing/preexpanding.c $(SRC_DIR)/parsing/preexpanding_utils.c \
	$(SRC_DIR)/parsing/lexing.c $(SRC_DIR)/parsing/lexing_w_token.c $(SRC_DIR)/parsing/lexing_utils.c \
	$(SRC_DIR)/parsing/parsing.c $(SRC_DIR)/parsing/parsing_utils.c $(SRC_DIR)/parsing/parsing_to_cmd_list.c \
	$(SRC_DIR)/parsing/expanding.c \
	$(SRC_DIR)/utils/free.c $(SRC_DIR)/utils/str_utils.c $(SRC_DIR)/utils/err_msg.c \
	$(SRC_DIR)/signals/signals.c $(SRC_DIR)/signals/signal_utils.c

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
