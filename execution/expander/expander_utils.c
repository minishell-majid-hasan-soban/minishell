/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:13:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/01 08:58:28 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strdupif_zero(t_ast *ast, char **expanded, int *size)
{
	if (*size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		if (!*expanded)
			ast->error = T_MALLOC;
		*size += 1;
	}
}

char	*ft_strjoin_arr(t_ast *ast, char **arr)
{
	char	*join;
	char	*tmp;
	int		i;

	i = 0;
	join = ft_strdup("");
	if (!join)
		return (ast->error = T_MALLOC, ft_free_args(arr), NULL);
	while (arr && arr[i])
	{
		tmp = join;
		join = ft_strjoin(join, arr[i]);
		if (!join)
			ast->error = T_MALLOC;
		(free(tmp), free(arr[i]));
		i++;
	}
	free(arr);
	return (join);
}
