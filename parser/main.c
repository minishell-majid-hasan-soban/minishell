#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pars.h"
#include "structs.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (new_size == 0 && ptr != NULL)
	{
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return malloc(new_size);
    void *new_ptr = malloc(new_size);
    if (new_ptr == NULL)
		return NULL;
	size_t bytes_to_copy = (old_size < new_size) * old_size + !(old_size < new_size) * new_size;
    ft_memcpy(new_ptr, ptr, bytes_to_copy);
    free(ptr);
   return new_ptr;
}


int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\f' || c == '\r' || c == '\v')
		return (1);
	return (0);
}

void skip_space(char **prompt)
{
	while(is_space(**prompt))
		((*prompt)++);
}

int add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	if(tokens->count + 1 > tokens->size)
	{
		ft_realloc(tokens->arr, tokens->size, tokens->size * 2);
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count] = (t_token){NULL, type, NULL};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT || type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	return 1;
}

int handle_seperator(char **prompt, t_token_arr *tokens)
{
	if (!ft_strncmp(*prompt, "<<", 2))
		return (add_separator(TOKEN_DLESS, prompt, tokens) && 1);
	else if (!ft_strncmp(*prompt, ">>", 2))
		return (add_separator(TOKEN_DGREAT, prompt, tokens) && 1);
	else if (!ft_strncmp(*prompt, "<", 1))
		return (add_separator(TOKEN_LESS, prompt, tokens) && 1);
	else if (!ft_strncmp(*prompt, ">", 1))
		return (add_separator(TOKEN_GREAT, prompt, tokens) && 1);
	else if (!ft_strncmp(*prompt, "&&", 2))
		return (add_separator(TOKEN_AND, prompt, tokens) && 1);
	else if (!ft_strncmp(*prompt, "||", 2))
		return (add_separator(TOKEN_OR, prompt, tokens) && 1);
	else
		return (add_separator(TOKEN_PIPE, prompt, tokens) && 1);
}

t_token_arr tokenize(char *prompt)
{
	t_token_arr res;
	int			ret;

	res = (t_token_arr){0, 32, 0};
	res.arr = ft_realloc(res.arr, res.size, res.size);

	while(*prompt)
	{
		if(is_space(*prompt))
			skip_space(&prompt);
		if(!ft_strncmp(prompt, "<", 1) || !ft_strncmp(prompt, "|", 1)
			|| !ft_strncmp(prompt, ">", 1) || !ft_strncmp(prompt, "&&", 2))
			ret = handle_seperator(&prompt, &res);
		else
		 ;
	}

	
}



int main() {

}
