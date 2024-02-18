/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:29:16 by amajid            #+#    #+#             */
/*   Updated: 2024/02/16 10:10:07 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "&&", 2)
		|| !ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	return (0);
}

int	is_separator_space(char *str)
{
	if (is_space(*str) || is_separator(str))
		return (1);
	return (0);
}

int	is_in_str(char *str, char c)
{
	while (*str && *str != c)
		str++;
	return (*str == c);
}

void	skip_space(char **prompt)
{
	while (is_space(**prompt))
		((*prompt)++);
}

int	add_token_word(t_token_arr *tokens, char *value)
{
	if (tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token),
				tokens->size * 2 * sizeof(t_token));
		if (!tokens->arr)
			return (-1);
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){value,
		TOKEN_WORD, T_NONE, NULL, NULL};
	return (1);
}
