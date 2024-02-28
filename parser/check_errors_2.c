/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:54:08 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 21:54:46 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_errors_part5(t_token_arr *tokens, t_ast *ast, t_err *d)
{
	int	ret;
	int	status;

	d->past_type = tokens->arr[d->index - 1 * (d->index > 0)].type;
	d->type = tokens->arr[d->index].type;
	ret = check_errors_part4(tokens, ast, d);
	if (ret == -1 || ret == 130 || ret == 42)
		return (ret);
	if (ret == 2)
		return (2);
	if ((d->past_type == TOKEN_AND || d->past_type == TOKEN_OR
			|| d->past_type == TOKEN_PIPE) && d->type == TOKEN_EOF)
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	if (d->past_type == TOKEN_DLESS && d->type == TOKEN_WORD)
	{
		tokens->arr[d->index - 1 * (d->index > 0)].heredoc_fd
			= init_here_doc(ast, tokens->arr[d->index].value, &status);
		if (status != 0)
			return (status);
	}
	d->index++;
	return (1);
}
