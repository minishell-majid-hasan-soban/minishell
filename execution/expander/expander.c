/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/18 10:28:34 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_dollar(t_ast *ast, char **arg, char **expanded);

size_t	ft_argslen(char **args)
{
	size_t	len;

	len = 0;
	while (args && args[len])
		len++;
	return (len);
}

static char	*get_substr(char *arg, int *i, bool skip)
{
	char	*sub;
	int		start;
	
	start = *i;
	if (skip)
		(*i)++;
	while (arg[*i] && arg[*i] != '\'' && arg[*i] != '\"' && arg[*i] != '$')
		(*i)++;
	if (skip && arg[*i])
		(*i)++;
	sub = ft_substr(arg, start, *i - start);
	return (sub);
}

static void append_squote(t_ast *ast, char **arg, char **expanded, bool expand)
{
	int		i;
	
	i = 1;
	while ((*arg)[i] && (*arg)[i] != '\'')
	{
		if ((*arg)[i] == '$' && expand)
			handle_dollar(ast, arg, expanded);
		else
			get_substr(*arg, &i, false);
		*arg += i;
		i = 0;
	}
}

static void append_dquote(t_ast *ast, char **arg, char **expanded)
{
	int		i;
	
	i = 1;
	while ((*arg)[i] && (*arg)[i] != '\"')
	{
		if ((*arg)[i] == '\'')
			append_squote(ast, arg, expanded, true);
		if ((*arg)[i] == '$')
			handle_dollar(ast, arg, expanded);
		else
			get_substr(*arg, &i, false);
		*arg += i;
		i = 0;
	}
}

static void handle_dollar(t_ast *ast, char **arg, char **expanded)
{
	char	*var;
	char	*tmp;
	char	*to_free;
	int		i;
	
	(*arg)++;
	if (**arg == '\0')
		var = ft_strdup("$");
	else if (**arg == '?')
		var = ft_itoa(ast->shell->exit_status);
	else
	{
		i = 0;
		while ((*arg)[i] && (ft_isalnum((*arg)[i]) || (*arg)[i] == '_'))
			i++;
		tmp = ft_substr(*arg, 0, i);
		*arg += i - 1;
		var = get_value(ast->shell->env, tmp);
		free(tmp);
	}
	*arg += 1;
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, var);
	(free(to_free), free(var));
}

char	*ft_expand_arg(t_ast *ast, char *arg)
{
	char	*expanded;
	char	*tmp;
	char	*to_free;
	int		i;

	expanded = ft_strdup("");
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(ast, &arg, &expanded, false);
		else if (*arg == '\"')
			append_dquote(ast, &arg, &expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded);
		else
		{
			i = 0;
			tmp = get_substr(arg, &i, false);
			arg += i;
			to_free = expanded;
			expanded = ft_strjoin(expanded, tmp);
			(free(to_free), free(tmp));
		}
	}
	return (expanded);
}

char	**ft_expand_args(t_ast *ast, char **args)
{
	char	**expanded;
	// char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	expanded = (char **)malloc(sizeof(char *) * (ft_argslen(args) + 1));
	while (args && args[i])
		expanded[j++] = ft_expand_arg(ast, args[i++]);
	expanded[j] = NULL;
	return (expanded);
}
