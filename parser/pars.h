#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include "../libft/libft.h"


#define TOKEN_COMMAND 0
#define TOKEN_ARGUMENT 1

typedef struct s_token {
    int type;   // TOKEN_COMMAND or TOKEN_ARGUMENT
    char *value;
} t_token;


#endif
