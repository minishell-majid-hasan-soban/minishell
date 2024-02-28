/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:59:34 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 18:09:31 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

t_redirection_type	get_redirection_type(t_token_type type)
{
	if (type == TOKEN_GREAT)
		return (R_OUTPUT);
	if (type == TOKEN_LESS)
		return (R_INPUT);
	if (type == TOKEN_DGREAT)
		return (R_APPEND);
	return (R_HEREDOC);
}


char	token_is_operator(t_token *curr_token)
{
	if ((*curr_token).type == TOKEN_AND || (*curr_token).type == TOKEN_OR
		|| (*curr_token).type == TOKEN_PIPE)
		return (1);
	return (0);
}

int	token_precedence(t_token *curr_token)
{
	if ((*curr_token).type == TOKEN_AND)
		return (1);
	else if ((*curr_token).type == TOKEN_OR)
		return (1);
	else if ((*curr_token).type == TOKEN_PIPE)
		return (2);
	return (1);
}

t_node_type	determine_node_type(t_token *op_token)
{
	if ((*op_token).type == TOKEN_AND)
		return (N_AND);
	else if ((*op_token).type == TOKEN_OR)
		return (N_OR);
	else if ((*op_token).type == TOKEN_PIPE)
		return (N_PIPE);
	return (-1);
}
