/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipper_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:05:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 18:25:33 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_dquote(t_ast *ast, char **arg, char **expanded)
{
	char	*tmp;
	char	*to_free;
	char	*to_free2;

	tmp = ft_strdup("\"");
	(*arg)++;
	while (**arg && **arg != '\"')
	{
		if (**arg == '$')
			handle_dollar(ast, arg, &tmp, true);
		else
			append_char(arg, &tmp);
	}
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, tmp);
	(free(to_free), free(tmp));
	to_free2 = *expanded;
	*expanded = ft_strjoin(*expanded, "\"");
	free(to_free2);
	(*arg)++;
}

char	*skip_quotes(t_ast *ast, char *str)
{
	char	*skiped;

	if (!str)
		return (NULL);
	skiped = ft_strdup("");
	while (*str)
	{
		if (*str == '"')
			handle_dquotes(ast, &str, &skiped);
		else if (*str == '\'')
			handle_squotes(ast, &str, &skiped);
		else if (*str == '$')
			handle_dollar(ast, str, &skiped, false);
		else
			handle_normal(&skiped, &str);
	}
}
