/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_seperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:58:17 by amajid            #+#    #+#             */
/*   Updated: 2024/02/27 18:23:44 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enum.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"

int	add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	t_token	*token;
	
	if (tokens->count + 1 > tokens->size)
	{
		token = ft_realloc(tokens->arr, tokens->size
				* sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		free(tokens->arr);
		tokens->arr = token;
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

typedef struct s_err
{
	t_token_type	past_type;
	t_token_type	type;
	long			index;
	long			is_in_op;
	long			j;
	bool			is_word_found;
	bool			is_after_parantheses;
}	t_err;

int	check_errors_part1(t_token_arr *tokens, t_err *d)
{
	if (d->is_in_op <= 0 && d->type == TOKEN_CP)
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	if (d->type == TOKEN_OP)
		d->is_in_op++;
	if (d->type == TOKEN_CP)
		d->is_in_op--;
	if (d->type == TOKEN_CP && d->is_in_op >= 0)
		d->is_after_parantheses = true;
	if (d->type == TOKEN_PIPE || d->type == TOKEN_AND || d->type == TOKEN_OR)
		d->is_after_parantheses = false;
	if ((d->type == TOKEN_AND || d->type == TOKEN_OR
			|| d->type == TOKEN_PIPE) && d->index == 0)
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	return (1);
}

int	check_errors_part2(t_token_arr *tokens, t_err *d)
{
	if (check_errors_part1(tokens, d) == -1)
		return (-1);
	if ((d->type == TOKEN_AND || d->type == TOKEN_OR
			|| d->type == TOKEN_PIPE || d->type == TOKEN_CP)
		&& (d->past_type == TOKEN_AND || d->past_type == TOKEN_OR
			|| d->past_type == TOKEN_PIPE || d->past_type == TOKEN_OP))
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	if (((d->index - 1 * (d->index > 0)) != d->index) && d->type != TOKEN_WORD
		&& (d->past_type == TOKEN_DGREAT
			|| d->past_type == TOKEN_DLESS || d->past_type == TOKEN_GREAT
			|| d->past_type == TOKEN_LESS) && !d->is_after_parantheses)
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	return (1);
}

int	check_errors_part3(t_token_arr *tokens, t_ast *ast, t_err *d)
{
	int	status;
	
	while (tokens->arr[d->index].type != TOKEN_EOF)
	{
		d->index++;
		if (!d->is_word_found && tokens->arr[d->index].type != TOKEN_WORD)
		{
			print_parse_error_near(&tokens->arr[d->index]);
			return (-1);
		}
		if (tokens->arr[d->index - 1].type == TOKEN_DLESS
			&& tokens->arr[d->index].type == TOKEN_WORD)
			{
				tokens->arr[d->index - 1].heredoc_fd
					= init_here_doc(ast, tokens->arr[d->index].value, &status);
				if (status != 0)
					return (status);
			}
		if (d->is_word_found && (tokens->arr[d->index].type == TOKEN_WORD
				|| tokens->arr[d->index].type == TOKEN_OP))
		{
			print_parse_error_near(&tokens->arr[d->index]);
			return (-1);
		}
		else if (d->is_word_found && (tokens->arr[d->index].type == TOKEN_AND
				|| tokens->arr[d->index].type == TOKEN_OR
				|| tokens->arr[d->index].type == TOKEN_PIPE
				|| tokens->arr[d->index].type == TOKEN_CP))
			break ;
		else if (d->is_word_found)
			d->is_word_found = false;
		if (!d->is_word_found && tokens->arr[d->index].type == TOKEN_WORD)
			d->is_word_found = true;
	}
	return (1);
}

int	check_errors_part4(t_token_arr *tokens, t_ast *ast, t_err *d)
{
	int	status;
	
	if (check_errors_part2(tokens, d) == -1)
		return (-1);
	if ((d->type == TOKEN_DGREAT || d->type == TOKEN_DLESS
			|| d->type == TOKEN_GREAT || d->type == TOKEN_LESS)
		&& d->is_after_parantheses)
	{
		d->j = d->index;
		d->is_word_found = false;
		status = check_errors_part3(tokens, ast, d);
		if (status == -1 || status == 130 || status == 42)
			return (status);
		return (2);
	}
	if (((d->type == TOKEN_OP && d->past_type != TOKEN_OP)
			&& (d->past_type != TOKEN_AND && d->past_type != TOKEN_OR
				&& d->past_type != TOKEN_PIPE))
		|| (d->type == TOKEN_WORD && d->past_type == TOKEN_CP))
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	return (1);
}

int	check_errors_part5(t_token_arr *tokens, t_ast *ast, t_err *d)
{
	int	ret;
	int	status;

	d->past_type = tokens->arr[d->index - 1 * (d->index > 0)].type;
	d->type = tokens->arr[d->index].type;
	ret = check_errors_part4(tokens, ast, d);
	if (ret == -1 || ret == 130 || ret == 42)
		return (ret);
	if (ret == 2)
		return (2);
	if ((d->past_type == TOKEN_AND || d->past_type == TOKEN_OR
			|| d->past_type == TOKEN_PIPE) && d->type == TOKEN_EOF)
	{
		print_parse_error_near(&tokens->arr[d->index]);
		return (-1);
	}
	if (d->past_type == TOKEN_DLESS && d->type == TOKEN_WORD)
	{
		tokens->arr[d->index - 1 * (d->index > 0)].heredoc_fd
			= init_here_doc(ast, tokens->arr[d->index].value, &status);
		if (status != 0)
			return (status);
	}
	d->index++;
	return (1);
}


int	check_errors_tokens(t_token_arr *tokens, t_ast *ast)
{
	t_err	d;
	int 	ret;

	d.is_in_op = 0;
	d.index = 0;
	d.is_after_parantheses = false;
	if (tokens->arr == NULL || tokens->count == 0)
		return (ft_putstr_fd(ALLOC_ERR, 2), -1);
	while (d.index < tokens->count)
	{
		ret = check_errors_part5(tokens, ast, &d);
		if (ret == -1 || ret == 130 || ret == 42)
			return (ret);
	}
	if (d.is_in_op > 0)
	{
		print_parse_error_near(&tokens->arr[d.index - 1]);
		return (-1);
	}
	return (1);
}
