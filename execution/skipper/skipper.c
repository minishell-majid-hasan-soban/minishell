/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:11:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/22 09:21:28 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes_work(char **dest, char **str, char quote)
{
	(*str)++;
	if (!is_in_str(*str, quote))
	{
		printf("minishell: syntax error, expected `%c`\n", quote);
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

char *skip_quotes(char *str)
{
	int i = 0;
	int j = 0;
	if (!str)
		return NULL;
	char *result = ft_calloc(ft_strlen(str) + 2, 1);
	char *tmp_str;
	char *tmp_result;
	while(str[i] && !is_space(str[i]))
	{
		if(str[i] == '\'' || str[i] == '"')
		{
			tmp_str = str + i;
			tmp_result = result + j;
			skip_quotes_work(&tmp_result, &tmp_str, str[i]);
			i += tmp_str - (str + i);
			j += tmp_result - (result + j);
			continue;
		}
		result[j] = str[i];
		i++;
		j++;
	}
	result[j] = 0;
	return result;
}

char	**ft_skip_args(char **args)
{
	int		i;
	char	**new_args;

	i = 0;
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
