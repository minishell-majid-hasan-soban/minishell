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
		libft/ft_isalnum.c      libft/ft_lstdelone.c    libft/ft_memmove.c \

EXEC =  builtins_cmd/args_utils.c		builtins_cmd/ft_perror.c		glober/glober.c					redirections/red_in.c \
		builtins_cmd/cd.c               builtins_cmd/handlers.c         handlers/error_handle.c         redirections/red_out.c \
		builtins_cmd/echo.c             builtins_cmd/pwd.c              handlers/exec_handles.c         redirections/her_doc.c \
		builtins_cmd/env.c              builtins_cmd/unset.c            builtins_cmd/env_utils1.c 		glober/globber_utils.c\
		builtins_cmd/exit.c             builtins_cmd/env_utils2.c 		exec_cmd/exec_ast.c             redirections/append.c \
		builtins_cmd/export.c           expander/expander.c				string_utils/string_utils.c		exec_cmd/exec_args.c \
		exec_cmd/exec_redir.c			handlers/exec_handles2.c 		handlers/syscall_handle.c		expander/handle_dollar.c \
		exec_cmd/exec_utils.c 			exec_cmd/exec_ast_utils.c 		exec_cmd/path_finder.c 			skipper/skipper.c \
		init_shell/init_ast.c			init_shell/init_env.c			init_shell/init_shell.c 		exec_cmd/run_cmd.c \
		free_stuff/free_shell.c			signals/signals.c				expander/underscore.c			builtins_cmd/cd_utils.c \
		glober/globber_utils2.c			expander/expander_utils.c \

PARSER =	parser/add_seperator.c	       \
			parser/ast_utils_1.c           parser/compute_atom.c          parser/tokenizer.c \
			parser/ast_utils_2.c           parser/free_funcs.c            parser/tokenizer_part1.c \
			parser/check_errors_1.c        parser/herdoc_init.c           parser/utils_1.c \
			parser/check_errors_2.c        parser/parse_expression.c      parser/utils_2.c \
			parser/check_errors_main.c     parser/printing_functions.c    parser/her_doc_utils.c

SRC = $(LIBFT) $(addprefix execution/, $(EXEC)) $(PARSER)  minishell.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RL = $(shell brew --prefix readline)
NAME = minishell
NAME_BONUS = minishell_bonus
INC = includes

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME) $(OBJ)
	@clear
	@echo "$(GREEN)$(NAME) has been created successfully!$(RESET)"

%.o : %.c $(INC)/minishell.h $(INC)/libft.h $(INC)/enum.h $(INC)/ast_handler.h $(INC)/builtins.h
	@$(CC) $(CFLAGS) -I$(RL)/include -I$(INC) -c $< -o $@
	@echo "$(BLUE)Compiling $<$ $(RESET)"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ)
	@$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME_BONUS) $(OBJ) 
	@echo "$(GREEN)$(NAME_BONUS) has been created successfully!$(RESET)"

clean:
	@rm -f $(OBJ)
	@echo "$(RED)Object files have been removed!$(RESET)"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "$(RED)$(NAME) has been removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re