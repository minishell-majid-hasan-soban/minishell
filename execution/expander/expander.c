/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/19 10:31:52 by hsobane          ###   ########.fr       */
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

static void append_squote(char **arg, char **expanded)
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

static void append_dquote(t_ast *ast, char **arg, char **expanded)
{
	char	*tmp;
	char	*to_free;
	
	tmp = ft_strdup("\"");
	(*arg)++;
	while (**arg && **arg != '\"')
	{
		if (**arg == '$')
			handle_dollar(ast, arg, &tmp);
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

static void handle_dollar(t_ast *ast, char **arg, char **expanded)
{
	char	*var;
	char	*to_free;
	int		i;
	
	(*arg)++;
	if (**arg == '\0')
		var = ft_strdup("$");
	else if (**arg == '?')
	{
		var = ft_itoa(ast->shell->exit_status);
		(*arg)++;
	}
	else
	{
		i = 0;
		while ((*arg)[i] && (ft_isalnum((*arg)[i]) || (*arg)[i] == '_'))
			i++;
		if (i == 0)
			var = ft_strdup("$");
		else
		{
			var = get_value(ast->shell->env, ft_substr(*arg, 0, i));
			*arg += i;
		}
	}
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, var);
	(free(to_free), free(var));
}

char	*ft_expand_arg(t_ast *ast, char *arg)
{
	char	*expanded;
	char	*to_free;

	expanded = ft_strdup("");
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(&arg, &expanded);
		else if (*arg == '\"')
			append_dquote(ast, &arg, &expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded);
		else
		{
			to_free = expanded;
			expanded = ft_strjoin(expanded, ft_substr(arg, 0, 1));
			free(to_free);
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
	expanded = (char **)malloc(sizeof(char *) * (ft_argslen(args) + 1));
	while (args && args[i])
	{
		tmp = ft_expand_arg(ast, args[i++]);
		expanded[j++] = skip_quotes(tmp);
		free(tmp);
		if (!expanded[j - 1])
			return (NULL);
	}
	expanded[j] = NULL;
	return (expanded);
}
