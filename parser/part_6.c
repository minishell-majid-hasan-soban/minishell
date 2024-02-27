/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:59:34 by amajid            #+#    #+#             */
/*   Updated: 2024/02/26 17:28:28 by amajid           ###   ########.fr       */
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

t_ast	*extract_command(t_token **curr_token)
{
	t_command		*command;
	t_ast			*ast;
	t_redirection	*redir;

	command = ft_calloc(1, sizeof(t_command));
	if (!command)
		return (NULL);
	if ((*curr_token)->type == TOKEN_OP)	// segfault input is minishell> "
	{
		free_command(command);
		(*curr_token)++;
		ast = parse_expression(curr_token, 1, true);
		if ((*curr_token)->type != TOKEN_CP)
		{
			printf("minishell: expected ')'\n");
			free_ast(ast);
			return (NULL);
		}
		(*curr_token)++;
		while ((*curr_token)->type != TOKEN_AND && (*curr_token)->type != TOKEN_OR
			&& (*curr_token)->type != TOKEN_PIPE && (*curr_token)->type != TOKEN_EOF
			&& (*curr_token)->type != TOKEN_OP && (*curr_token)->type != TOKEN_CP)
		{
			if ((*curr_token)->type != TOKEN_WORD)
			{
				redir = create_redirection((*curr_token), *((*curr_token) + 1));
				if (redir == NULL)
				{
					printf(ALLOC_ERR);
					free_ast(ast);
					return (NULL);
				}
				add_back_redirection(&ast->redirections, redir);
				(*curr_token)++;
			}
			else
			{
				print_parse_error_near((*curr_token));
				free_ast(ast);
				return (NULL);
			}
			(*curr_token)++;
		}
		return (ast);
	}
	while ((*curr_token)->type != TOKEN_AND && (*curr_token)->type != TOKEN_OR
		&& (*curr_token)->type != TOKEN_PIPE && (*curr_token)->type != TOKEN_EOF
		&& (*curr_token)->type != TOKEN_OP && (*curr_token)->type != TOKEN_CP)
	{
		if ((*curr_token)->type == TOKEN_WORD)
			add_node_arg(command, (*curr_token)->value);
		else
		{
			redir = create_redirection((*curr_token), *((*curr_token) + 1));
			if (redir == NULL)
			{
				free_command(command);
				return (NULL);
			}
			add_back_redirection(&command->redirections, redir);
			(*curr_token)++;
		}
		(*curr_token)++;
	}
	return (create_ast_node(N_CMD, command));
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
