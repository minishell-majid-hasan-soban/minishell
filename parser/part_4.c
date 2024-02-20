/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:46:09 by amajid            #+#    #+#             */
/*   Updated: 2024/02/20 18:08:38 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_arr	tokenize(char *prompt, t_ast *ast)
{
	t_token_arr	tokens;
	int			ret;

	(void)ast;
	tokens = (t_token_arr){0, 1000, 0};
	tokens.arr = ft_realloc(tokens.arr, 0, tokens.size * sizeof(t_token));
	if (!tokens.arr)
	{
		tokens = (t_token_arr){0};
		return (tokens);
	}
	while (*prompt)
	{
		if (is_space(*prompt))
			skip_space(&prompt);
		if (is_separator(prompt))
			ret = handle_seperator(&prompt, &tokens);
		else if (*prompt)
			ret = add_word(&prompt, &tokens);
		if (ret == -1)
		{
			free_token_arr(&tokens);
			tokens = (t_token_arr){0};
			return (tokens);
		}
	}
	add_eof(&tokens);
	if (tokens.arr[tokens.count - 2 * (tokens.count > 1)].type == TOKEN_AND
		|| tokens.arr[tokens.count - 2 * (tokens.count > 1)].type == TOKEN_OR
		|| tokens.arr[tokens.count - 2 * (tokens.count > 1)].type == TOKEN_PIPE)
	{
		print_parse_error_near(&tokens.arr[tokens.count - 1 * (tokens.count > 0)]);
		free_token_arr(&tokens);
		return (tokens);
	}
	return (tokens);
}

void	free_token_arr(t_token_arr *tokens)
{
	int	i;

	i = -1;
	while (++i < tokens->count)
		if (tokens->arr[i].value && tokens->arr[i].type != TOKEN_EOF)
			free(tokens->arr[i].value);
	free(tokens->arr);
	(*tokens) = (t_token_arr){0};
}

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
	if(command->args)
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
