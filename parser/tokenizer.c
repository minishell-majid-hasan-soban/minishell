/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:46:09 by amajid            #+#    #+#             */
/*   Updated: 2024/02/29 17:07:40 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenizer_part1(char **prompt, t_token_arr *tokens)
{
	int	ret;

	ret = 1;
	if (is_space(**prompt))
		skip_space(prompt);
	if (is_separator(*prompt))
		ret = handle_seperator(prompt, tokens);
	else if (**prompt)
		ret = add_word(prompt, tokens);
	tokens->status = ret;
	return (ret);
}

int	tokenizer_part2(t_token_arr *tokens)
{
	t_token	*token;

	token = ft_realloc(tokens->arr, 0, tokens->size * sizeof(t_token));
	free(tokens->arr);
	tokens->arr = token;
	if (!tokens->arr)
	{
		*tokens = (t_token_arr){0};
		tokens->status = -1;
		return (-1);
	}
	return (1);
}

t_token_arr	tokenize(char *prompt)
{
	t_token_arr	tokens;
	int			ret;

	tokens = (t_token_arr){.arr = 0, .size = 2, .count = 0, .status = 1};
	if (tokenizer_part2(&tokens) == -1)
		return (tokens);
	while (*prompt)
	{
		ret = tokenizer_part1(&prompt, &tokens);
		if (ret == -1 || ret == -2)
		{
			free_token_arr(&tokens);
			tokens = (t_token_arr){.arr = 0,
				.size = 0, .count = 0, .status = ret};
			return (tokens);
		}
	}
	add_eof(&tokens);
	return (tokens);
}
