/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globber_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:08:24 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 20:08:29 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted(char *arg, char target, bool all)
{
	int		i;
	bool	squote;
	bool	dquote;

	i = 0;
	squote = false;
	dquote = false;
	while (arg[i])
	{
		if (arg[i] == '\'')
			squote = !squote;
		if (arg[i] == '\"')
			dquote = !dquote;
		if (arg[i] == target && (squote || dquote) && !all)
			return (true);
		if (arg[i] == target && !squote && !dquote && all)
			return (false);
		i++;
	}
	if (!all)
		return (false);
	return (true);
}
