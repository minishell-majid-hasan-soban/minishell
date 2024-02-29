/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:13:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 20:29:33 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strdupif_zero(char **expanded, int *size)
{
	if (*size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		*size += 1;
	}
}

char	*ft_strjoin_arr(char **arr)
{
	char	*join;
	char	*tmp;
	int		i;

	i = 0;
	join = ft_strdup("");
	while (arr && arr[i])
	{
		tmp = join;
		join = ft_strjoin(join, arr[i]);
		(free(tmp), free(arr[i]));
		i++;
	}
	free(arr);
	return (join);
}
