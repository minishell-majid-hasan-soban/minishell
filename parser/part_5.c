/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:51:47 by amajid            #+#    #+#             */
/*   Updated: 2024/02/16 10:10:40 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *command)
{
	free_args(command);
	command->args = NULL;
	free_redirections(command->redirections);
	command->redirections = NULL;
	free(command);
}

void	free_ast(t_ast* node)
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->command)
		free_command(node->command);
	free(node);
}

int	add_node_arg(t_command *command, char *arg)
{
	if (command->arg_size == 0)
	{
		command->args = ft_realloc(command->args, 0, sizeof(char *) * 2);
		if (!command->args)
			return (-1);
		command->arg_size = 2;
	}
	if (command->arg_count + 1 > command->arg_size)
	{
		command->args = ft_realloc(command->args, command->arg_size
				* sizeof(char *), command->arg_size * 2 * sizeof(char *));
		if (!command->args)
			return (-1);
		command->arg_size *= 2;
	}
	command->args[command->arg_count++] = ft_strdup(arg);
	return (1);
}

t_redirection	*create_redirection(t_redirection_type type, t_token file)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	if (file.type != TOKEN_WORD)
	{
		print_parse_error_near(&file);
		free(redir);
		return (NULL);
	}
	redir->file = strdup(file.value);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

int	add_back_redirection(t_command *command, t_redirection *redir)
{
	t_redirection	*curr;

	if (!(command->redirections))
	{
		(command->redirections) = redir;
		return (1);
	}
	curr = command->redirections;
	while (curr->next)
		curr = curr->next;
	curr->next = redir;
	return (1);
}
