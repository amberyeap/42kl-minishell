# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/11 14:30:25 by ayeap             #+#    #+#              #
#    Updated: 2024/06/14 11:25:22 by ayeap            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc
NAME = libft.a
RM = rm -f

LIBFT_SRC_FILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
			ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
			ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
			ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

FT_PRINTF = ft_printf/libftprintf.a
GNL = get_next_line/gnl.a

SRC_DIR = src
LIBFT_SRC = $(addprefix $(SRC_DIR)/, $(LIBFT_SRC_FILES))
OBJ_DIR = obj
OBJ = $(LIBFT_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJ_DIR) $(OBJ) $(FT_PRINTF) $(GNL)
	@ar -rcs $(NAME) $(OBJ) $(FT_PRINTF) $(GNL)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(FT_PRINTF):
	@make -C ft_printf

$(GNL):
	@make -C get_next_line

all: bonus $(NAME)

clean:
	@$(RM) $(OBJ)
	@make clean -C ft_printf
	@make clean -C get_next_line

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ft_printf
	@make fclean -C get_next_line

re: fclean $(NAME)

.PHONY: all, clean, fclean, re, bonus
