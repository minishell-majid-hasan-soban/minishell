BLUE = \033[0;34m
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
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
		builtins_cmd/echo.c             builtins_cmd/pwd.c              handlers/exec_handles.c         redirections/her_doc.c \
		builtins_cmd/env.c              builtins_cmd/unset.c            builtins_cmd/env_utis.c			pipe/pipe.c \
		builtins_cmd/exit.c             exec_cmd/exec_ast.c             redirections/append.c           string_utils/string_utils1.c \
		builtins_cmd/export.c           expander/expander.c				string_utils/string_utils.c		exec_cmd/exec_args.c \
		exec_cmd/exec_redir.c			handlers/exec_handles2.c 
PARSER =	parser/part_1.c				parser/part_2.c					parser/part_3.c					parser/part_4.c \
		parser/part_5.c					parser/part_6.c					parser/part_7.c 
SRC = $(LIBFT) $(addprefix execution/, $(EXEC)) $(PARSER) test.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RL = $(shell brew --prefix readline)
NAME = minishell
INC = includes

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)$(NAME) has been created successfully!$(RESET)"

%.o : %.c $(INC)/minishell.h $(INC)/libft.h $(INC)/enum.h
	@$(CC) $(CFLAGS) -I$(RL)/include -I$(INC) -c $< -o $@
	@echo "$(BLUE)Compiling $<$ $(RESET)"

clean:
	@rm -f $(OBJ)
	@echo "$(RED)Object files have been removed!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) has been removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re