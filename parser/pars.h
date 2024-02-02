#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include "../libft/libft.h"


#define TOKEN_command 0
#define TOKEN_argument 1
#define TOKEN_pipe 2
#define TOKEN_red_out 3
#define TOKEN_red_in 4
#define TOKEN_here_doc_in 5
#define TOKEN_here_doc_out 6
#define TOKEN_env_var 7
#define TOKEN_exit_status 8







typedef struct s_token {
    int type;   // TOKEN_COMMAND or TOKEN_ARGUMENT
    char *value;
} t_token;


#endif
