SRC = main.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
RL = $(shell brew --prefix readline)
NAME = minishell
INC = includes

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME) $(OBJ)

%.o : %.c $(INC)/minishell.h $(INC)/libft.h $(INC)/enum.h
	$(CC) $(CFLAGS) -I$(RL)/inc -I$(INC) -c $< -o $@ 

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all clean fclean re