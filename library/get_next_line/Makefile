CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

NAME = gnl.a

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

$(NAME): $(OBJ)
	@ar -rcs $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re