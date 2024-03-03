/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 08:36:35 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:01:40 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_whitespace(char *str, int i)
{
	return (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13));
}

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (!to_find)
		return (0);
	if (!to_find[0])
		return (str);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i + j] && str[i + j] == to_find[j])
			j++;
		if (!to_find[j])
			return (str + i);
		i++;
	}
	return (0);
}
