/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 15:52:54 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_squote(char **arg, char **expanded)
{
	int		i;
	char	*tmp;

	i = 1;
	while ((*arg)[i] && (*arg)[i] != '\'')
		i++;
	tmp = *expanded;
	*expanded = ft_strjoin(*expanded, ft_substr(*arg, 0, i + 1));
	free(tmp);
	*arg += i + ((*arg)[i] != '\0');
}

static void	append_dquote(t_ast *ast, char **arg, char **expanded)
{
	char	*tmp;
	char	*to_free;

	tmp = ft_strdup("\"");
	(*arg)++;
	while (**arg && **arg != '\"')
	{
		if (**arg == '$')
			handle_dollar(ast, arg, &tmp, true);
		else
		{
			to_free = tmp;
			tmp = ft_strjoin(tmp, ft_substr(*arg, 0, 1));
			free(to_free);
			(*arg)++;
		}
	}
	to_free = tmp;
	tmp = ft_strjoin(tmp, "\"");
	free(to_free);
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, tmp);
	(free(to_free), free(tmp));
	(*arg)++;
}

char	*ft_expand_arg(t_ast *ast, char *arg)
{
	char	*expanded;
	char	*to_free;
	char	*tmp;

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

	i = 0;
	j = 0;
	expanded = ft_calloc(ft_argslen(args) + 1, sizeof(char *));
	while (args && args[i])
	{
		tmp = ft_expand_arg(ast, args[i++]);
		expanded[j++] = skip_quotes(tmp);
		free(tmp);
		if (!expanded[j - 1])
			return (ft_free_args(expanded), NULL);
	}
	return (expanded);
}
