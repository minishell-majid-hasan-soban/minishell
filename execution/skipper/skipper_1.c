/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skipper_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:05:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 08:08:42 by hsobane          ###   ########.fr       */
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
	*str += i + ((*str)[i] != '\0');
	if (quote == '"')
		subword = ft_expand_arg(ast, tmp);
	else
		subword = ft_strdup(tmp);
	free(tmp);
	tmp = *word;
	*word = ft_strjoin(*word, subword);
	(free(tmp), free(subword));
}

static char	**get_expanded_words(t_ast *ast, char *str)
{
	char	*word;
	char	**words;
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
	words = ft_split(word, ' ');
	free(word);
	return (words);
}

char	**ft_expand_args(t_ast *ast, char **args)
{
	char	**expanded;
	char	**tmp2;
	char	**tmp;
	int		i;

	if (!args)
		return (NULL);
	i = 0;
	expanded = ft_calloc(1, sizeof(char *));
	while (args && args[i])
	{
		tmp = get_expanded_words(ast, args[i]);
		if (!tmp)
			return (ft_free_args(expanded), NULL);
		tmp2 = expanded;
		expanded = ft_strsjoin(expanded, tmp);
		(free(tmp), free(tmp2));
	}
	return (expanded);
}
