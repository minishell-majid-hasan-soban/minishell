/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:07:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 08:07:52 by hsobane          ###   ########.fr       */
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
			printf("minishell: parse error near '%s'\n", (*curr_token)->value);
		else
			print_parse_error_near((*curr_token));
		free_ast(l_ast);
		return (NULL);
	}
	return (l_ast);
}

typedef struct s_parse_exp
{
	t_ast	*l_ast;
	t_token	*op_token;
	t_ast	*r_ast;
	t_ast	*new_ast;
} t_parse_exp;

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
	t_redirection	*tmp;

	tmp = redirection;
	while (tmp)
	{
		printf(" (%s -> %s)", redirection_type_to_string(tmp->type),
			tmp->file);
		tmp = tmp->next;
	}
	// printf("\n");
}

void	print_command_args(t_command *cmd)
{
	char	**args;

	if (cmd == NULL)
		return ;
	args = cmd->args;
	if (args)
	{
		printf("- ");
		while (*args)
		{
			printf("%s ", *args);
			args++;
		}
	}
	print_redirections(cmd->redirections);
	// printf("\n");
}

void	print_ast(const t_ast *node, const char *prefix, int is_left)
{
	char	next_prefix[256];
	int		i;

	if (node == NULL)
		return ;
	printf("%s", prefix);
	i = -1;
	while (++i < 256)
		next_prefix[i] = 0;
	if (is_left)
		printf("%s", "|--");
	else
		printf("%s", "\\--");
	if (node->type == N_CMD)
	{
		print_command_args(node->command);
		if(node->redirections)
			print_redirections(node->redirections);
		printf("\n");
	}
	else
	{
		if ((node->type == N_PIPE))
			printf("%s", "Pipe");
		else if (node->type == N_AND)
			printf("%s", "AND");
		else
			printf("%s", "OR");
		if (node->redirections)
		{
			print_redirections(node->redirections);
		}
		printf("\n");
	}
	ft_strlcat(next_prefix, prefix, 256);
	if (is_left)
		ft_strlcat(next_prefix, "|   ", 256);
	else
		ft_strlcat(next_prefix, "    ", 256);
	print_ast(node->left, next_prefix, 1);
	print_ast(node->right, next_prefix, 0);
}
