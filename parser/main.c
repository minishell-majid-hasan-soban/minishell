#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pars.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (new_size == 0 && ptr != NULL)
	{
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return malloc(new_size);
    void *new_ptr = ft_calloc(new_size, 1);
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


int	is_str(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int	is_separator_space(char *str)
{
	if (is_space(*str)
		|| !ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "&&", 2))
		return (1);
	return (0);
}

int is_separator(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "&&", 2))
		return (1);
	return 0;
}

int is_in_str(char *str, char c)
{
	while(*str && *str != c)
		str++;
	return (*str == c);
}

void skip_space(char **prompt)
{
	while(is_space(**prompt))
		((*prompt)++);
}

int add_token_word(t_token_arr *tokens, char *value)
{
	if(tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){value, TOKEN_WORD, NULL};
	return (1);
}

int add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	if(tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, type, NULL};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT || type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	return 1;
}

int handle_seperator(char **prompt, t_token_arr *tokens)
{
	if (!ft_strncmp(*prompt, "<<", 2))
		return (add_separator(TOKEN_DLESS, prompt, tokens));
	else if (!ft_strncmp(*prompt, ">>", 2))
		return (add_separator(TOKEN_DGREAT, prompt, tokens));
	else if (!ft_strncmp(*prompt, "&&", 2))
		return (add_separator(TOKEN_AND, prompt, tokens));
	else if (!ft_strncmp(*prompt, "||", 2))
		return (add_separator(TOKEN_OR, prompt, tokens));
	else if (!ft_strncmp(*prompt, "|", 1))
		return (add_separator(TOKEN_PIPE, prompt, tokens));
	else if (!ft_strncmp(*prompt, "<", 1))
		return (add_separator(TOKEN_LESS, prompt, tokens));
	else if (!ft_strncmp(*prompt, ">", 1))
		return (add_separator(TOKEN_GREAT, prompt, tokens));
	return -1;
}


int skip_str(char **str, char quote)
{
	(*str)++;
	if(!is_in_str(*str, quote))
		return -1;

	while(**str != quote)
		(*str)++;
	(*str)++;
	return 1;
}

int add_word(char **prompt, t_token_arr *tokens)
{
	char	*prompt_ptr;
	char	*word;

	prompt_ptr = *prompt;
	while (*prompt_ptr && !is_separator_space(prompt_ptr))
		if (is_str(*prompt_ptr))
		{
			if (!skip_str(&prompt_ptr, *prompt_ptr))
				return (-1);
		}
		else
			prompt_ptr++;
	word = ft_substr(*prompt, 0, prompt_ptr - (*prompt));
	if (!word)
		return (0);
	printf("prompt_ptr - (*prompt) = %ld\n", prompt_ptr - (*prompt));
	*prompt += prompt_ptr - (*prompt);
	return (add_token_word(tokens, word));
}


t_token_arr tokenize(char *prompt)
{
	t_token_arr tokens;
	int			ret;

	tokens = (t_token_arr){0, 1000, 0};
	tokens.arr = ft_realloc(tokens.arr, 0, tokens.size * sizeof(t_token));
	if(!tokens.arr)
	{
		free(tokens.arr);
		tokens = (t_token_arr){0};
		return tokens;
	}
	while(*prompt)
	{
		if(is_space(*prompt))
			skip_space(&prompt);
		if(is_separator(prompt))
		{
			ret = handle_seperator(&prompt, &tokens);
			//printf("here 1\n");
		}
		else
		{
		 	ret = add_word(&prompt, &tokens);
			//printf("here 2\n");

		}
		if(ret == -1)
		{
			free(tokens.arr);
			tokens = (t_token_arr){0};
			return tokens;
		}
	}
	return tokens;
}


void free_token_arr(t_token_arr *tokens)
{
	int i = -1;

	while(++i < tokens->count)
		if(tokens->arr[i].value)
			free(tokens->arr[i].value);
	free(tokens->arr);
}


int main() {
	char *str = "ls | grep thing > test < thing << >> || cd || ygerfuy | test && \"   test &&  pipe | < > << >> || \"|";
	t_token_arr tokens = tokenize(str);
	
	int i = -1;
	char types[][100] = 
	{
		"TOKEN_WORD",
		"TOKEN_PIPE",
		"TOKEN_GREAT",
		"TOKEN_LESS",
		"TOKEN_DGREAT",
		"TOKEN_DLESS",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_NEWLINE",
		"TOKEN_EOF"
	};
	while (++i < tokens.count)
		printf("token type = %s, value = %s\n", types[tokens.arr[i].type], tokens.arr[i].value);
	free_token_arr(&tokens);
}
