/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:25:25 by amajid            #+#    #+#             */
/*   Updated: 2024/02/19 20:00:08 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	bytes_to_copy;

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = ft_calloc(new_size, 1);
	if (new_ptr == NULL)
		return (NULL);
	bytes_to_copy = (old_size < new_size) * old_size
		+ !(old_size < new_size) * new_size;
	ft_memcpy(new_ptr, ptr, bytes_to_copy);
	free(ptr);
	return (new_ptr);
}

void	print_parse_error_near(t_token *token)
{
	char	types[15][100];

	ft_strlcpy(types[0], "", 100);
	ft_strlcpy(types[1], "|", 100);
	ft_strlcpy(types[2], ">", 100);
	ft_strlcpy(types[3], "<", 100);
	ft_strlcpy(types[4], ">>", 100);
	ft_strlcpy(types[5], "<<", 100);
	ft_strlcpy(types[6], "&&", 100);
	ft_strlcpy(types[7], "||", 100);
	ft_strlcpy(types[8], "NEWLINE", 100);
	ft_strlcpy(types[9], "(", 100);
	ft_strlcpy(types[10], ")", 100);
	ft_strlcpy(types[11], "newline", 100);
	printf("minishell: syntax error near unexpected token '%s'\n",
		types[token->type]);
}

void	print_token(t_token *token)
{
	char	types[15][100];

	ft_strlcpy(types[0], "TOKEN_WORD", 100);
	ft_strlcpy(types[1], "TOKEN_PIPE", 100);
	ft_strlcpy(types[2], "TOKEN_GREAT", 100);
	ft_strlcpy(types[3], "TOKEN_LESS", 100);
	ft_strlcpy(types[4], "TOKEN_DGREAT", 100);
	ft_strlcpy(types[5], "TOKEN_DLESS", 100);
	ft_strlcpy(types[6], "TOKEN_AND", 100);
	ft_strlcpy(types[7], "TOKEN_OR", 100);
	ft_strlcpy(types[8], "TOKEN_NEWLINE", 100);
	ft_strlcpy(types[9], "TOKEN_OP", 100);
	ft_strlcpy(types[10], "TOKEN_CP", 100);
	ft_strlcpy(types[11], "TOKEN_EOF", 100);
	printf("token type = %s, value = %s\n", types[token->type], token->value);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\f' || c == '\r' || c == '\v')
		return (1);
	return (0);
}

int	is_str(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}
