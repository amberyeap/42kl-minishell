# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/27 15:01:42 by ayeap             #+#    #+#              #
#    Updated: 2024/11/27 15:01:42 by ayeap            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -rf
RL = -lreadline

NAME = minishell
LIBFT = library/libft.a

SRC_DIR = src
SRC_FILES = builtins/builtin.c builtins/cd_utils.c builtins/cd.c builtins/echo.c \
			builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c \
			builtins/unset.c builtins/utils.c \
			env/env_sort.c env/env.c \
			expansion/expansion.c \
			main/main_utils.c main/main.c \
			parsing/free_nodes.c parsing/parse_redir.c parsing/parse_simple_command.c \
			parsing/parser_utils.c parsing/parser.c \
			signals/signals.c \
			tokenization/tokenizer_create_tokens.c tokenization/tokenizer_quotes.c \
			tokenization/tokenizer_unquoted.c tokenization/tokenizer_utils.c \
			tokenization/tokenizer.c tokenization/utils.c \
			exec/exec.c exec/exec_utils.c exec/heredoc.c exec/redir.c \
			exec/exec_utils2.c exec/redir_utils.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(RL)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIBFT):
	make -C library

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) $(OBJ_DIR)
	make clean -C library

fclean: clean
	$(RM) $(NAME)
	make fclean -C library

re: fclean all

.PHONY: all clean fclean re
