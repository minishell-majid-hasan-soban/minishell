SRC = main.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = $(shell brew --prefix readline)/lib
INCS = $(shell brew --prefix readline)/include
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -L $(LIBS) -lreadline $(CFLAGS) -o $(NAME) $(OBJ)

%.o : %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all clean fclean re