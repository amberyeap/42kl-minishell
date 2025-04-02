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

SRC = expansion.c free_nodes.c main_utils.c main.c \
	parse_redir.c parse_simple_command.c parser_utils.c parser.c \
	signals.c tokenizer_create_tokens.c tokenizer_quotes.c tokenizer_unquoted.c \
	tokenizer_utils.c tokenizer.c utils.c
OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(RL)

$(LIBFT):
	make -C library

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)
	make clean -C library

fclean: clean
	$(RM) $(NAME)
	make fclean -C library

re: fclean all

.PHONY: all clean fclean re
