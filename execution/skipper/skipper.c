/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:11:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 15:09:09 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes_work(char **dest, char **str, char quote)
{
	(*str)++;
	if (!is_in_str(*str, quote))
	{
		ft_putstr_fd("minishell: syntax error, expected `", 2);
		ft_putchar_fd(quote, 2);
		ft_putstr_fd("`\n", 2);
		return (-1);
	}
	while (**str != quote)
	{
		*(*dest) = *(*str);
		(*str)++;
		(*dest)++;
	}
	(*str)++;
	return (1);
}

static	int	handle_quotes(char *dest, char *str, int *i, int *j)
{
	char	*tmp_str;
	char	*tmp_dest;

	tmp_str = str + *i;
	tmp_dest = dest + *j;
	if (skip_quotes_work(&tmp_dest, &tmp_str, str[*i]))
	{
		*i += tmp_str - (str + *i);
		*j += tmp_dest - (dest + *j);
	}
	else
		return (-1);
	return (1);
}

char	*skip_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	i = 0;
	j = 0;
	while (str[i] && !is_space(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
			handle_quotes(result, str, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = 0;
	return (result);
}

char	**ft_skip_args(char **args)
{
	int		i;
	char	**new_args;

	i = 0;
	if (!args)
		return (NULL);
	new_args = ft_calloc(ft_argslen(args) + 1, sizeof(char *));
	if (!new_args)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	while (args[i])
	{
		new_args[i] = skip_quotes(args[i]);
		if (!new_args[i])
			return (ft_free_args(new_args), NULL);
		i++;
	}
	return (new_args);
}
