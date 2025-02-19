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
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
RL = -lreadline

NAME = minishell
LIBFT = library/libft.a

SRC = main.c parser.c parser_utils.c signals.c tokenizer.c tokenizer_utils.c utils.c
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