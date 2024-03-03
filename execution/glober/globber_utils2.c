/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globber_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:08:24 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:08:46 by hsobane          ###   ########.fr       */
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

bool	is_not_quoted(char *s, int i)
{
	bool	squoted;
	bool	dquoted;
	int		j;

	if (s[i] != '*')
		return (false);
	squoted = false;
	dquoted = false;
	j = 0;
	while (j < i)
	{
		if (s[j] == '\'')
			squoted = !squoted;
		if (s[j] == '\"')
			dquoted = !dquoted;
		j++;
	}
	return (!squoted && !dquoted);
}
