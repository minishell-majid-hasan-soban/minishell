/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 22:23:23 by amajid            #+#    #+#             */
/*   Updated: 2024/02/11 22:32:29 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

t_ast	*parse_expression(t_token **curr_token,
int min_precedence, char is_in_op)
{
	t_ast	*l_ast;
	t_token	*op_token;
	t_ast	*r_ast;
	t_ast	*new_ast;

	l_ast = extract_command(curr_token);
	if (l_ast == NULL)
		return (NULL);
	if ((*curr_token)->type == TOKEN_CP && is_in_op)
		return (l_ast);
	if ((*curr_token)->type != TOKEN_EOF && !token_is_operator(*curr_token))
	{
		if ((*curr_token)->type == TOKEN_WORD)
			printf("minishell: parse error near '%s'\n", (*curr_token)->value);
		else
			print_parse_error_near((*curr_token));
		free_ast(l_ast);
		return (NULL);
	}
	while ((*curr_token)->type != TOKEN_EOF && token_is_operator(*curr_token)
		&& token_precedence(*curr_token) >= min_precedence)
	{
		op_token = *curr_token;
		*curr_token = ++(*curr_token);
		r_ast = parse_expression(curr_token,
				token_precedence(op_token) + 1, is_in_op);
		if (r_ast == NULL)
		{
			free_ast(l_ast);
			return (NULL);
		}
		new_ast = create_ast_node(determine_node_type(op_token), NULL);
		new_ast->left = l_ast;
		new_ast->right = r_ast;
		l_ast = new_ast;
	}
	return (l_ast);
}

char	*redirection_type_to_string(t_redirection_type type)
{
	if (type == R_INPUT)
		return ("R_INPUT");
	if (type == R_OUTPUT)
		return ("R_OUTPUT");
	if (type == R_APPEND)
		return ("R_APPEND");
	if (type == R_HEREDOC)
		return ("R_HEREDOC");
	return (NULL);
}

void	print_redirections(t_redirection *redirection)
{
	while (redirection != NULL)
	{
		printf(" (%s -> %s)", redirection_type_to_string(redirection->type),
			redirection->file);
		redirection = redirection->next;
	}
	printf("\n");
}

void	print_command_args(t_command	*cmd)
{
	int	i;

	i = -1;
	if (cmd && cmd->args)
	{
		printf("- ");
		while (++i < cmd->arg_count)
		{
			printf("%s ", cmd->args[i]);
		}
		print_redirections(cmd->redirections);
		printf("\n");
	}
	else
	{
		printf("-Empty Command");
		print_redirections(cmd->redirections);
		printf("\n");
	}
}

void	print_ast(const t_ast *node, const char *prefix, int is_left)
{
	char	next_prefix[256];
	int		i;

	if (node == NULL)
		return ;
	i = -1;
	while (++i < 256)
		next_prefix[i] = 0;
	printf("%s", prefix);
	if (is_left)
		printf("%s", "|--");
	else
		printf("%s", "\\--");
	if (node->type == N_CMD)
		print_command_args(node->command);
	else
	{
		if ((node->type == N_PIPE))
			printf("%s\n", "Pipe");
		else if (node->type == N_AND)
			printf("%s\n", "AND");
		else
			printf("%s\n", "OR");
	}
	ft_strlcat(next_prefix, prefix, 256);
	if (is_left)
		ft_strlcat(next_prefix, "|   ", 256);
	else
		ft_strlcat(next_prefix, "    ", 256);
	print_ast(node->left, next_prefix, 1);
	print_ast(node->right, next_prefix, 0);
}
