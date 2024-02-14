/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:40:35 by amajid            #+#    #+#             */
/*   Updated: 2024/02/12 16:30:20 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

int	add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	t_token_type	past_token_type;

	if (tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size
				* sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if (!tokens->arr)
			return (-1);
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, type, T_NONE, NULL, NULL};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT
		|| type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	if ((type == TOKEN_AND || type == TOKEN_OR
			|| type == TOKEN_PIPE) && tokens->count == 1)
	{
		print_parse_error_near(&tokens->arr[tokens->count - 1]);
		return -1;
	}
	past_token_type = tokens->arr[tokens->count - 2
		* (tokens->count >= 2)].type;
	if ((type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE || type == TOKEN_CP)
		&& (past_token_type == TOKEN_AND || past_token_type == TOKEN_OR
			|| past_token_type == TOKEN_PIPE || past_token_type == TOKEN_OP))
	{
		print_parse_error_near(&tokens->arr[tokens->count - 1]);
		return (-1);
	}
	return (1);
}

int	add_eof(t_token_arr *tokens)
{
	if (tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size
				* sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if (!tokens->arr)
			return (-1);
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL,
		TOKEN_EOF, T_NONE, NULL, NULL};
	return (1);
}

int	handle_seperator(char **prompt, t_token_arr *tokens)
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
	else if (!ft_strncmp(*prompt, "(", 1))
		return (add_separator(TOKEN_OP, prompt, tokens));
	else if (!ft_strncmp(*prompt, ")", 1))
		return (add_separator(TOKEN_CP, prompt, tokens));
	return (-1);
}

int	skip_str(char **str, char quote)
{
	(*str)++;
	if (!is_in_str(*str, quote))
	{
		printf("minishell: syntax error, expected `%c`\n", quote);
		return (-1);
	}
	while (**str != quote)
		(*str)++;
	(*str)++;
	return (1);
}

int	add_word(char **prompt, t_token_arr *tokens)
{
	char	*prompt_ptr;
	char	*word;

	prompt_ptr = *prompt;
	while (*prompt_ptr && !is_separator_space(prompt_ptr))
	{
		if (is_str(*prompt_ptr))
		{
			if (skip_str(&prompt_ptr, *prompt_ptr) == -1)
				return (-1);
		}
		else
			prompt_ptr++;
	}
	word = ft_substr(*prompt, 0, prompt_ptr - (*prompt));
	if (!word)
		return (0);
	*prompt += prompt_ptr - (*prompt);
	return (add_token_word(tokens, word));
}