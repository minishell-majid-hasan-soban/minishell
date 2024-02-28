/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:58:13 by amajid            #+#    #+#             */
/*   Updated: 2024/02/28 21:59:29 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(t_command *command)
{
	char	**args;

	if (command == NULL)
		return ;
	args = command->args;
	while (args && *args)
	{
		free(*args);
		args++;
	}
	if (command->args)
		free(command->args);
}

void	free_redirections(t_redirection *redirections)
{
	if (redirections == NULL)
		return ;
	free_redirections(redirections->next);
	free(redirections->file);
	free(redirections);
}

void	free_token_arr(t_token_arr	*tokens)
{
	int	i;

	if (!tokens->arr)
		return ;
	i = -1;
	while (++i < tokens->count)
		if (tokens->arr[i].value && tokens->arr[i].type != TOKEN_EOF)
			free(tokens->arr[i].value);
	free(tokens->arr);
	tokens->arr = NULL;
}

void	free_command(t_command *command)
{
	free_args(command);
	command->args = NULL;
	free_redirections(command->redirections);
	command->redirections = NULL;
	free(command);
}

void	free_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->command)
		free_command(node->command);
	free(node);
}
