NAME := libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
MKDIR := mkdir -p
RM = rm -f

LIBFT00_DIR := libft00
FTPRINTF_DIR := ft_printf
GNL_DIR := get_next_line
OBJ_DIR := obj

LIBFT00_SRC := ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
		ft_isprint.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c \
		ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strrchr.c ft_tolower.c \
		ft_toupper.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
		ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
		ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c ft_free_arr.c \
		ft_isspace.c ft_strndup.c ft_strcmp.c ft_is_empty_or_redirect.c ft_atoll.c

FT_PRINTF_SRC := ft_printf.c write_char.c write_str.c write_int.c \
		write_uint_base.c write_pointer.c

GNL_SRC := get_next_line.c get_next_line_utils.c

SRCS := $(addprefix $(LIBFT00_DIR)/, $(LIBFT00_SRC)) \
	$(addprefix $(FTPRINTF_DIR)/, $(FT_PRINTF_SRC)) \
	$(addprefix $(GNL_DIR)/, $(GNL_SRC))

OBJECTS = 	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))

INCLUDES = -I.

all: $(NAME)

$(NAME): $(OBJECTS)
	@ar rcs $(NAME) $(OBJECTS)

#$(MKDIR) $(dir $@) ==mkdir -p obj/ft_printf/
#dir list directory contents
#-p make parent directories as needed
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
