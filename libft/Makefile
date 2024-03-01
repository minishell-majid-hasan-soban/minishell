BLUE = \033[0;34m
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
YELLO = \033[0;93m

SRC =	ft_atoi.c               ft_lstclear.c           ft_memset.c             ft_strlcpy.c \
		ft_bzero.c              ft_lstdelone.c          ft_putchar_fd.c         ft_strlen.c \
		ft_calloc.c             ft_lstiter.c            ft_putendl_fd.c         ft_strmapi.c \
		ft_isalnum.c            ft_lstlast.c            ft_putnbr_fd.c          ft_strncmp.c \
		ft_isalpha.c            ft_lstmap.c             ft_putstr_fd.c          ft_strnstr.c \
		ft_isascii.c            ft_lstnew.c             ft_split.c              ft_strrchr.c \
		ft_isdigit.c            ft_lstsize.c            ft_strchr.c             ft_strtrim.c \
		ft_isprint.c            ft_memchr.c             ft_strdup.c             ft_substr.c \
		ft_itoa.c               ft_memcmp.c             ft_striteri.c           ft_tolower.c \
		ft_lstadd_back.c        ft_memcpy.c             ft_strjoin.c            ft_toupper.c \
		ft_lstadd_front.c       ft_memmove.c            ft_strlcat.c 

OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
INC = ../includes

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@echo "$(GREEN)$(NAME) has been created successfully!$(RESET)"

%.o: %.c $(INC)/libft.h
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLO)Compiling $<$ $(RESET)"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re