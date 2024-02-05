/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/05 17:04:58 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_argslen(char **args)
{
	size_t	len;

	len = 0;
	while (args[len])
		len++;
	return (len);
}

static char	*get_substring(char *arg, int *i)
{
	char	*sub;
	int		start;
	
	start = *i;
	(*i)++;
	while (arg[*i] && arg[*i] != '\'' && arg[*i] != '\"' && arg[*i] != '$')
		(*i)++;
	sub = ft_substr(arg, start, *i - start);
	return (sub);
}

static void append_squote(t_ast *ast, char *arg, int *i, char **expanded)
{
	int		start;
	char	*tmp;
	char	*to_free;
	
	start = *i;
	(*i)++;
	while (arg[*i] && arg[*i] != '\'')
		(*i)++;
	tmp = ft_substr(arg, start, *i - start);
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, tmp);
	free(to_free);
	free(tmp);
	(*i)++;
}

static void append_dquote(t_ast *ast, char *arg, int *i, char **expanded)
{
	int		start;
	
	start = *i;
	(*i)++;
	while (arg[*i] && arg[*i] != '\"')
	{
		if (arg[*i] == '\'')
			append_squote(ast, arg, i, expanded);
		if (arg[*i] == '$')
			handle_dollar(ast, arg, i, expanded);
		else
			get_substring(arg, i);
	}
}

static void handle_dollar(t_ast *ast, char *arg, int *i, char **expanded)
{
	char	*var;
	char	*tmp;
	char	*to_free;
	int		start;
	
	(*i)++;
	if (arg[*i] == '\0' && ++(*i))
		var = ft_strdup("$");
	else if (arg[*i] == '?' && ++(*i))
		var = ft_itoa(ast->shell->exit_status);
	else
	{
		start = *i;
		while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
			(*i)++;
		tmp = ft_substr(arg, start, *i - start);
		var = ft_getenv(ast->shell->env, tmp);
		free(tmp);
	}
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, var);
	free(to_free);
	free(var);
}

static char	*ft_expand_arg(t_ast *ast, char *arg)
{
	char	*expanded;
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	expanded = ft_strdup("");
	while (arg[i])
	{
		if (arg[i] == '\'')
			append_squote(ast, arg, &i, &expanded);
		else if (arg[i] == '\"')
			append_dquote(ast, arg, &i, &expanded);
		else if (arg[i] == '$')
			handle_dollar(ast, arg, &i, &expanded);
	}
	expanded = ft_strjoin(expanded, arg + i);
	return (expanded);
}

char	**ft_expand_args(t_ast *ast, char **args)
{
	char	**expanded;
	int		i;
	int		j;

	i = 0;
	j = 0;
	expanded = (char **)malloc(sizeof(char *) * (ft_argslen(args) + 1));
	while (args[i])
		expanded[j++] = ft_expand_arg(ast, args[i++]);
	expanded[j] = NULL;
	ft_free_args(args);
	return (expanded);
}
