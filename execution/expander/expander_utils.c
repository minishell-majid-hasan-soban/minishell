/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:13:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 20:18:14 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strdupif_zero(char **expanded, int *size)
{
	if (*size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		*size++;
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

char	**ft_expand_arg(t_ast *ast, char *arg)
{
	char	**expanded;

	if (!arg)
		return (NULL);
	expanded = ft_calloc(2, sizeof(char *));
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(&arg, expanded);
		else if (*arg == '\"')
			append_dquote(ast, &arg, expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded, false);
		else
			append_char(&arg, expanded);
	}
	return (expanded);
}
