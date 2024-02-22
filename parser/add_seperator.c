/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_seperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:58:17 by amajid            #+#    #+#             */
/*   Updated: 2024/02/22 21:47:59 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enum.h"
#include "minishell.h"
#include "parser.h"

int	add_sep_part_1(t_token_type type, char **prompt, t_token_arr *tokens)
{
	if (tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size
				* sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if (!tokens->arr)
			return (-1);
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, type, T_NONE, -1};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT
		|| type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	return (1);
}

int	add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{

	if (add_sep_part_1(type, prompt, tokens) == -1)
		return (-1);

	return (1);
}


int	check_errors_tokens(t_token_arr *tokens, t_ast *ast)
{
	t_token_type	past_type;
	t_token_type	type;
	long			index;
	int				is_in_op;
	long			j;
	bool			is_word_found;
	bool			is_after_parantheses;

	is_in_op = 0;
	index = 0;
	is_after_parantheses = false;
	while (index < tokens->count)
	{
		past_type = tokens->arr[index - 1 * (index > 0)].type;
		type = tokens->arr[index].type;
		if (is_in_op <= 0 && type == TOKEN_CP)
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		if (type == TOKEN_OP)
			is_in_op++;
		if (type == TOKEN_CP)
			is_in_op--;
		if (type == TOKEN_CP && is_in_op == 0)
			is_after_parantheses = true;
		if (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR)
			is_after_parantheses = false;
		if ((type == TOKEN_AND || type == TOKEN_OR
				|| type == TOKEN_PIPE) && index == 0)
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		if ((type == TOKEN_AND || type == TOKEN_OR
				|| type == TOKEN_PIPE || type == TOKEN_CP)
			&& (past_type == TOKEN_AND || past_type == TOKEN_OR
				|| past_type == TOKEN_PIPE || past_type == TOKEN_OP))
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		if (((index - 1 * (index > 0)) != index) && type != TOKEN_WORD
			&& (past_type == TOKEN_DGREAT
				|| past_type == TOKEN_DLESS || past_type == TOKEN_GREAT
				|| past_type == TOKEN_LESS) && !is_after_parantheses)
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		print_token(&tokens->arr[index]);
		if ((type == TOKEN_DGREAT || type == TOKEN_DLESS
				|| type == TOKEN_GREAT || type == TOKEN_LESS)
			&& is_after_parantheses)
		{
			j = index;
			is_word_found = false;
			while (tokens->arr[j].type != TOKEN_EOF)
			{
				j++;
				print_token(&tokens->arr[j]);
				if (tokens->arr[j - 1].type == TOKEN_DLESS
					&& tokens->arr[j].type == TOKEN_WORD)
					tokens->arr[j - 1].heredoc_fd
						= init_here_doc(ast, tokens->arr[j].value);
				if (is_word_found && tokens->arr[j].type == TOKEN_WORD)
				{
					print_parse_error_near(&tokens->arr[j]);
					return (-1);
				}
				else if (is_word_found && (tokens->arr[j].type == TOKEN_AND
						|| tokens->arr[j].type == TOKEN_OR
						|| tokens->arr[j].type == TOKEN_PIPE))
					break ;
				else if (is_word_found)
					is_word_found = false;
				if (!is_word_found && tokens->arr[j].type == TOKEN_WORD)
					is_word_found = true;
			}
			index = j;
		}
		printf("----\n");
		if (((type == TOKEN_OP && past_type != TOKEN_OP)
				&& (past_type != TOKEN_AND && past_type != TOKEN_OR
					&& past_type != TOKEN_PIPE))
			|| (type == TOKEN_WORD && past_type == TOKEN_CP))
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		if ((past_type == TOKEN_AND || past_type == TOKEN_OR
				|| past_type == TOKEN_PIPE) && type == TOKEN_EOF)
		{
			print_parse_error_near(&tokens->arr[index]);
			return (-1);
		}
		if (past_type == TOKEN_DLESS && type == TOKEN_WORD)
			tokens->arr[index - 1 * (index > 0)].heredoc_fd
				= init_here_doc(ast, tokens->arr[index].value);
		index++;
	}
	if (is_in_op > 0)
	{
		print_parse_error_near(&tokens->arr[index - 1]);
		return (-1);
	}
	return (1);
}
