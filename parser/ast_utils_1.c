/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:51:47 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 22:05:48 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_ast_node(t_node_type type, t_command *command)
{
	t_ast	*node;

	node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (node != NULL)
	{
		node->type = type;
		node->command = command;
		node->left = NULL;
		node->right = NULL;
	}
	return (node);
}

int	add_node_arg(t_command *command, char *arg)
{
	char	**new_args;

	new_args = malloc(sizeof(char *) * (ft_argslen(command->args) + 2));
	if (!new_args)
		return (1);
	ft_memcpy(new_args, command->args,
		sizeof(char *) * ft_argslen(command->args));
	new_args[ft_argslen(command->args)] = strdup(arg);
	if (!new_args[ft_argslen(command->args)])
		return (free(new_args), 1);
	new_args[ft_argslen(command->args) + 1] = NULL;
	free(command->args);
	command->args = new_args;
	return (0);
}

t_redirection	*create_redirection(t_token *token_op, t_token file)
{
	t_redirection	*redir;

	redir = ft_calloc(1, sizeof(t_redirection));
	if (!redir)
		return (NULL);
	if (file.type != TOKEN_WORD)
	{
		print_parse_error_near(&file);
		free(redir);
		return (NULL);
	}
	redir->file = strdup(file.value);
	redir->type = get_redirection_type(token_op->type);
	redir->next = NULL;
	redir->prev = NULL;
	redir->heredoc_fd = token_op->heredoc_fd;
	return (redir);
}

int	add_back_redirection(t_redirection **head, t_redirection *redir)
{
	t_redirection	*curr;

	if (!((*head)))
	{
		(*head) = redir;
		return (1);
	}
	curr = (*head);
	while (curr->next)
		curr = curr->next;
	curr->next = redir;
	return (1);
}
