/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:51:47 by amajid            #+#    #+#             */
/*   Updated: 2024/02/20 18:56:09 by amajid           ###   ########.fr       */
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
	char	**new_args;

	new_args = malloc(sizeof(char *) * (ft_argslen(command->args) + 2));
	if (!new_args)
		return (1);
	ft_memcpy(new_args, command->args, sizeof(char *) * ft_argslen(command->args));
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

int	add_back_redirection(t_command *command, t_redirection *redir)
{
	t_redirection	*curr;

	if (!(command->redirections))
	{
		command->redirections = redir;
		return (1);
	}
	curr = command->redirections;
	while (curr->next)
		curr = curr->next;
	curr->next = redir;
	return (1);
}
