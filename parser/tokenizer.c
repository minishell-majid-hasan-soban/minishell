/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:46:09 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 22:22:15 by amajid           ###   ########.fr       */
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
	return (ret);
}

t_token_arr	tokenize(char *prompt)
{
	t_token_arr	tokens;
	t_token		*token;
	int			ret;

	tokens = (t_token_arr){.arr = 0, .size = 2, .count = 0};
	token = ft_realloc(tokens.arr, 0, tokens.size * sizeof(t_token));
	free(tokens.arr);
	tokens.arr = token;
	if (!tokens.arr)
	{
		tokens = (t_token_arr){0};
		return (tokens);
	}
	while (*prompt)
	{
		ret = tokenizer_part1(&prompt, &tokens);
		if (ret == -1)
		{
			free_token_arr(&tokens);
			tokens = (t_token_arr){0};
			return (tokens);
		}
	}
	add_eof(&tokens);
	return (tokens);
}
