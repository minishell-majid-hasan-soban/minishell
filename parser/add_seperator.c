/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_seperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:58:17 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 21:53:44 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enum.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"

int	add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	t_token	*token;

	if (tokens->count + 1 > tokens->size)
	{
		token = ft_realloc(tokens->arr, tokens->size
				* sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		free(tokens->arr);
		tokens->arr = token;
		if (!tokens->arr)
			return (-1);
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, type, T_NONE, -1};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT
		|| type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	return (1);
}
