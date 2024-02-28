/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:32:03 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 22:20:38 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_errors_tokens(t_token_arr *tokens, t_ast *ast)
{
	t_err	d;
	int		ret;

	d.is_in_op = 0;
	d.index = 0;
	d.is_after_parantheses = false;
	if (tokens->arr == NULL || tokens->count == 0)
		return (ft_putstr_fd(ALLOC_ERR, 2), -1);
	while (d.index < tokens->count)
	{
		ret = check_errors_part5(tokens, ast, &d);
		if (ret == -1 || ret == 130 || ret == 42)
			return (ret);
	}
	if (d.is_in_op > 0)
	{
		print_parse_error_near(&tokens->arr[d.index - 1]);
		return (-1);
	}
	return (1);
}
