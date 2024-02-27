/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:40:35 by amajid            #+#    #+#             */
/*   Updated: 2024/02/26 17:11:30 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enum.h"
#include "minishell.h"

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
		TOKEN_EOF, T_NONE, -1};
	// printf("word2 = %s\n, count = %ld\n", tokens->arr[0].value, tokens->count);
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
		ft_putstr_fd("minishell: syntax error, expected `", 2);
		ft_putchar_fd(quote, 2);
		ft_putstr_fd("`\n", 2);
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
	// printf("word = %s\n", word);
	return (add_token_word(tokens, word));
}
