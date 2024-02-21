/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 03:04:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 10:43:54 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

size_t	ft_argslen(char **args)
{
	size_t	len;

	len = 0;
	while (args && args[len])
		len++;
	return (len);
}

void	ft_free_args(char **args)
{
	int	j;

	j = 0;
	while (args && args[j])
		free(args[j++]);
	free(args);
}
