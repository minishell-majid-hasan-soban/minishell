/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipper_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:05:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 07:16:19 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	get_quoted_word(t_ast *ast, char **str, char **word)
{
	char	*subword;
	char	*tmp;
	char	quote;
	int		i;

	i = 0;
	quote = **str;
	(*str)++;
	while ((*str)[i] && (*str)[i] != quote)
		i++;
	tmp = ft_substr(*str, 0, i);
	*str += i + 1;
	if (quote == '"')
		subword = ft_expand_arg(ast, tmp);
	else
		subword = ft_strdup(tmp);
	free(tmp);
	tmp = *word;
	*word = ft_strjoin(*word, subword);
	(free(tmp), free(subword));
}

static char	*get_word(t_ast *ast, char *str)
{
	char	*word;
	char	*tmp;

	word = ft_strdup("");
	while (*str)
	{
		if (*str == "\'" || *str == "\"")
			get_quoted_word(ast, &str, &word);
		else if (*str == '$')
			handle_dollar(ast, &str, &word, false);
		else
			append_char(&str, &word);
	}
	return (word);
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
