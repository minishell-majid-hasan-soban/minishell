/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 11:45:48 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_squote(char **arg, char **expanded)
{
	int		i;
	char	*tmp;
	char	*to_free;

	i = 1;
	while ((*arg)[i] && (*arg)[i] != '\'')
		i++;
	tmp = *expanded;
	to_free = ft_substr(*arg, 0, i + 1);
	*expanded = ft_strjoin(*expanded, to_free);
	(free(to_free), free(tmp));
	*arg += i + ((*arg)[i] != '\0');
}

static void	append_char(char **arg, char **expanded)
{
	char	*to_free;
	char	*to_free2;

	to_free = *expanded;
	to_free2 = ft_substr(*arg, 0, 1);
	*expanded = ft_strjoin(*expanded, to_free2);
	(free(to_free), free(to_free2));
	(*arg)++;
}

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

char	*ft_expand_arg(t_ast *ast, char *arg)
{
	char	*expanded;
	char	*to_free;
	char	*tmp;

	if (!arg)
		return (NULL);
	expanded = ft_strdup("");
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(&arg, &expanded);
		else if (*arg == '\"')
			append_dquote(ast, &arg, &expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded, false);
		else
		{
			to_free = expanded;
			tmp = ft_substr(arg, 0, 1);
			expanded = ft_strjoin(expanded, tmp);
			(free(to_free), free(tmp));
			arg++;
		}
	}
	return (expanded);
}

char	**ft_expand_args(t_ast *ast, char **args)
{
	char	**expanded;
	char	*tmp;
	int		i;
	int		j;

	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	expanded = ft_calloc(ft_argslen(args) + 1, sizeof(char *));
	while (args && args[i])
	{
		tmp = ft_expand_arg(ast, args[i]);
		if (!tmp)
			return (ft_free_args(expanded), NULL);
		expanded[j] = tmp;
		i++;
		j++;
	}
	return (expanded);
}
