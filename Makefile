BLUE = \033[0;34m
GREEN = \033[0;32m
RED = \033[0;91m
RESET = \033[0m

EXEC =	execution/builtins_cmd/cd.c						execution/expander/expander_utils.c \
		execution/builtins_cmd/cd_utils.c				execution/expander/handle_dollar.c \
		execution/builtins_cmd/echo.c					execution/expander/underscore.c \
		execution/builtins_cmd/env.c					execution/free_stuff/free_shell.c \
		execution/builtins_cmd/env_utils1.c				execution/glober/globber_utils.c \
		execution/builtins_cmd/env_utils2.c				execution/glober/globber_utils2.c \
		execution/builtins_cmd/exit.c					execution/glober/glober.c \
		execution/builtins_cmd/export.c					execution/handlers/exec_handles.c \
		execution/builtins_cmd/handlers.c				execution/handlers/exec_handles2.c \
		execution/builtins_cmd/pwd.c					execution/handlers/syscall_handle.c \
		execution/builtins_cmd/unset.c					execution/init_shell/init_ast.c \
		execution/exec_cmd/exec_args.c					execution/init_shell/init_env.c \
		execution/exec_cmd/exec_ast.c					execution/init_shell/init_shell.c \
		execution/exec_cmd/exec_ast_utils.c				execution/redirections/exec_redir.c \
		execution/exec_cmd/exec_utils.c					execution/redirections/redirection_utils.c \
		execution/exec_cmd/path_finder.c				execution/signals/signals.c \
		execution/exec_cmd/run_cmd.c					execution/skipper/skipper.c \
		execution/expander/expander.c					execution/string_utils/string_utils.c 

PARSER =	parser/add_seperator.c			parser/compute_atom.c			parser/tokenizer.c \
			parser/ast_utils_1.c			parser/free_funcs.c				parser/tokenizer_part1.c \
			parser/ast_utils_2.c			parser/her_doc_utils.c			parser/utils_1.c \
			parser/check_errors_1.c			parser/herdoc_init.c			parser/utils_2.c \
			parser/check_errors_2.c			parser/parse_expression.c \
			parser/check_errors_main.c		parser/printing_functions.c 

LIBFT = libft/libft.a 
SRC = $(EXEC) $(PARSER) minishell.c 
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
RL = $(shell brew --prefix readline)
NAME = minishell
NAME_BONUS = minishell_bonus
INC = includes
HEADER = $(INC)/minishell.h $(INC)/enum.h $(INC)/ast_handler.h $(INC)/builtins.h $(INC)/parser.h

all: libft $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)
	@echo "$(GREEN)$(NAME) has been created successfully!$(RESET)"

%.o: %.c $(HEADER)
	@$(CC) -I$(INC) -I$(RL)/include $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling $<$ $(RESET)"

libft:
	@make -C libft
	@echo "$(GREEN)Libft has been created successfully!$(RESET)"

bonus: libft $(NAME_BONUS)

$(NAME_BONUS): $(OBJ) $(LIBFT)
	@$(CC) -I$(INC) -L $(RL)/lib -lreadline $(CFLAGS) -o $(NAME_BONUS) $(OBJ) $(LIBFT)
	@echo "$(GREEN)$(NAME_BONUS) has been created successfully!$(RESET)"

clean:
	@rm -f $(OBJ)
	@make -C libft clean
	@echo "$(RED)Object files have been removed!$(RESET)"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@make -C libft fclean
	@echo "$(RED)$(NAME) has been removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re libft bonus