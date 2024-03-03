/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expression.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:07:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 16:23:25 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_expression_part1(t_token **curr_token, bool in_op)
{
	t_ast	*l_ast;

	if (!(*curr_token))
		return (NULL);
	l_ast = extract_command(curr_token);
	if (l_ast == NULL)
		return (NULL);
	if ((*curr_token)->type == TOKEN_CP && in_op)
		return (l_ast);
	if ((*curr_token)->type != TOKEN_EOF && !token_is_operator(*curr_token))
	{
		if ((*curr_token)->type == TOKEN_WORD)
		{
			ft_putstr_fd("minishell: parse error near '\n", 2);
			ft_putendl_fd((*curr_token)->value, 2);
		}
		else
			print_parse_error_near((*curr_token));
		free_ast(l_ast);
		return (NULL);
	}
	return (l_ast);
}

int	parse_expression_while_guts(t_token **curr_token,
		bool in_op, t_parse_exp *data)
{
	data->op_token = *curr_token;
	*curr_token = ++(*curr_token);
	data->r_ast = parse_expression(curr_token,
			token_precedence(data->op_token) + 1, in_op);
	if (data->r_ast == NULL)
	{
		free_ast(data->l_ast);
		return (-1);
	}
	data->new_ast = create_ast_node(determine_node_type(data->op_token),
			ft_calloc(sizeof(t_command), 1));
	data->new_ast->left = data->l_ast;
	data->new_ast->right = data->r_ast;
	data->l_ast = data->new_ast;
	return (1);
}

t_ast	*parse_expression(t_token **curr_token, int min_precedence, bool in_op)
{
	t_parse_exp	data;

	data.l_ast = parse_expression_part1(curr_token, in_op);
	if (data.l_ast == NULL)
		return (NULL);
	while ((*curr_token)->type != TOKEN_EOF && token_is_operator(*curr_token)
		&& token_precedence(*curr_token) >= min_precedence)
	{
		if (parse_expression_while_guts(curr_token,
				in_op, &data) == -1)
			return (NULL);
	}
	return (data.l_ast);
}
