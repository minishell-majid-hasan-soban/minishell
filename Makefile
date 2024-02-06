LIBFT = libft/ft_isalpha.c      libft/ft_lstiter.c      libft/ft_memset.c       libft/ft_strjoin.c      libft/ft_substr.c \
	 	libft/ft_isascii.c      libft/ft_lstlast.c      libft/ft_putchar_fd.c   libft/ft_strlcat.c      libft/ft_tolower.c \
	 	libft/ft_isdigit.c      libft/ft_lstmap.c       libft/ft_putendl_fd.c   libft/ft_strlcpy.c      libft/ft_toupper.c \
	 	libft/ft_isprint.c      libft/ft_lstnew.c       libft/ft_putnbr_fd.c    libft/ft_strlen.c       libft/ft_strtrim.c \
		libft/ft_itoa.c         libft/ft_lstsize.c      libft/ft_putstr_fd.c    libft/ft_strmapi.c		libft/ft_striteri.c \
		libft/ft_atoi.c         libft/ft_lstadd_back.c  libft/ft_memchr.c       libft/ft_split.c        libft/ft_strncmp.c \
		libft/ft_bzero.c        libft/ft_lstadd_front.c libft/ft_memcmp.c       libft/ft_strchr.c       libft/ft_strnstr.c \
		libft/ft_calloc.c       libft/ft_lstclear.c     libft/ft_memcpy.c       libft/ft_strdup.c       libft/ft_strrchr.c \
		libft/ft_isalnum.c      libft/ft_lstdelone.c    libft/ft_memmove.c      
EXEC =  builtins_cmd/args_utils.c		builtins_cmd/ft_perror.c		glober/glober.c					redirections/red_in.c \
		builtins_cmd/cd.c               builtins_cmd/handlers.c         handlers/error_handle.c         redirections/red_out.c \
		builtins_cmd/echo.c             builtins_cmd/pwd.c              handlers/exec_handles.c         string_utils/ft_split.c \
		builtins_cmd/env.c              builtins_cmd/unset.c            pipe/pipe.c                     string_utils/string_utils.c \
		builtins_cmd/exit.c             exec_cmd/exec_ast.c             redirections/append.c           string_utils/string_utils1.c \
		builtins_cmd/export.c           expander/expander.c             redirections/her_doc.c
SRC = $(LIBFT) $(addprefix execution/, $(EXEC)) test.c
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