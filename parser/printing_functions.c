/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:02:00 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 22:02:25 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
