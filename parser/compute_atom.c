/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_atom.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 18:09:00 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 18:09:26 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*extract_command_part1(t_token **curr_token, t_ast *ast)
{
	t_redirection	*redir;

	while ((*curr_token)->type != TOKEN_AND && (*curr_token)->type != TOKEN_OR
		&& (*curr_token)->type != TOKEN_PIPE && (*curr_token)->type != TOKEN_EOF
		&& (*curr_token)->type != TOKEN_OP && (*curr_token)->type != TOKEN_CP)
	{
		if ((*curr_token)->type != TOKEN_WORD)
		{
			redir = create_redirection((*curr_token), *((*curr_token) + 1));
			if (redir == NULL)
				return (ft_putstr_fd(ALLOC_ERR, 2), free_ast(ast), (NULL));
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

int	extract_command_part2(t_token **curr_token, t_command *command)
{
	t_redirection	*redir;

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
				return (-1);
			}
			add_back_redirection(&command->redirections, redir);
			(*curr_token)++;
		}
		(*curr_token)++;
	}
	return (1);
}



t_ast	*extract_command(t_token **curr_token)
{
	t_command		*command;
	t_ast			*ast;

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
		return (extract_command_part1(curr_token, ast));
	}
	if (extract_command_part2(curr_token, command) == -1)
		return (NULL);
	return (create_ast_node(N_CMD, command));
}
