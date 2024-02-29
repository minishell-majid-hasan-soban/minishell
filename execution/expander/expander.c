/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 13:59:41 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_char(char **arg, char **expanded)
{
	char	*to_free;
	char	*tmp;
	int		size;

	size = ft_argslen(expanded);
	if (size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		size++;
	}
	tmp = expanded[size - 1];
	to_free = ft_substr(*arg, 0, 1);
	expanded[size - 1] = ft_strjoin(expanded[size - 1], to_free);
	(free(tmp), free(to_free));
	(*arg)++;
}

static void	append_squote(char **arg, char **expanded)
{
	int		i;
	char	*tmp;
	char	*to_free;
	int		size;

	i = 0;
	(*arg)++;
	while ((*arg)[i] && (*arg)[i] != '\'')
		i++;
	size = ft_argslen(expanded);
	if (size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		size++;
	}
	tmp = expanded[size - 1];
	to_free = ft_substr(*arg, 0, i);
	expanded[size - 1] = ft_strjoin(expanded[size - 1], to_free);
	(free(tmp), free(to_free));
	(*arg) += i + ((*arg)[i] != '\0');
}

static char	*ft_strjoin_arr(char **arr)
{
	char	*join;
	char	*tmp;
	int		i;

	i = 0;
	join = ft_strdup("");
	while (arr && arr[i])
	{
		tmp = join;
		join = ft_strjoin(join, arr[i]);
		(free(tmp), free(arr[i]));
		i++;
	}
	free(arr);
	return (join);
}

static void	append_dquote(t_ast *ast, char **arg, char **expanded)
{
	char	**tmp;
	char	*join;
	char	*to_free;
	int		size;

	(*arg)++;
	tmp = ft_calloc(2, sizeof(char *));
	while (**arg && **arg != '\"')
	{
		if (**arg == '$')
			handle_dollar(ast, arg, &tmp, true);
		else
			append_char(arg, tmp);
	}
	join = ft_strjoin_arr(tmp);
	size = ft_argslen(expanded);
	if (size == 0)
	{
		free(*expanded);
		*expanded = ft_strdup("");
		size++;
	}
	to_free = expanded[size - 1];
	expanded[size - 1] = ft_strjoin(expanded[size - 1], join);
	(free(to_free), free(join));
	(*arg) += **arg != '\0';
}

char	**ft_expand_arg(t_ast *ast, char *arg)
{
	char	**expanded;

	if (!arg)
		return (NULL);
	expanded = ft_calloc(2, sizeof(char *));
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(&arg, expanded);
		else if (*arg == '\"')
			append_dquote(ast, &arg, expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded, false);
		else
			append_char(&arg, expanded);
	}
	return (expanded);
}

// static void	get_quoted_word(t_ast *ast, char **str, char ***word)
// {
// 	char	**subword;
// 	char	*tmp;
// 	char	quote;
// 	int		i;

// 	i = 0;
// 	quote = **str;
// 	(*str)++;
// 	while ((*str)[i] && (*str)[i] != quote)
// 		i++;
// 	tmp = ft_substr(*str, 0, i);
// 	*str += i + ((*str)[i] != '\0');
// 	if (quote == '"')
// 		subword = ft_expand_arg(ast, tmp);
// 	else
// 		subword = ft_strdup(tmp);
// 	free(tmp);
// 	tmp = *word;
// 	*word = ft_strjoin(*word, subword);
// 	(free(tmp), free(subword));
// }

// static char	**get_expanded_word(t_ast *ast, char *str)
// {
// 	char	**word;

// 	word = ft_calloc(1, sizeof(char *));
// 	while (*str)
// 	{
// 		if (*str == '\'' || *str == '"')
// 			get_quoted_word(ast, &str, &word);
// 		else if (*str == '$')
// 			handle_dollar(ast, &str, &word, false);
// 		else
// 			append_char(&str, &word);
// 	}
// 	return (word);
// }

void print_args(char **args, char *name)
{
	int	i;

	i = 0;
	printf("%s:\n", name);
	while (args && args[i])
	{
		printf("args[%d]: %s\n", i, args[i]);
		i++;
	}
	printf("\n");
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
	expanded = ft_calloc(2, sizeof(char *));
	while (args && args[i])
	{
		tmp = ft_expand_arg(ast, args[i]);
		// print_args(tmp, "tmp");
		// print_args(expanded, "expanded");
		tmp2 = expanded;
		expanded = ft_strsjoin(expanded, tmp);
		(free(tmp2), free(tmp));
		i++;
	}
	return (expanded);
}

// char	**ft_expand_args(t_ast *ast, char **args)
// {
// 	char	**expanded;
// 	char	*tmp;
// 	int		i;
// 	int		j;

// 	if (!args)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	expanded = ft_calloc(ft_argslen(args) + 1, sizeof(char *));
// 	while (args && args[i])
// 	{
// 		tmp = ft_expand_arg(ast, args[i]);
// 		if (!tmp)
// 			return (ft_free_args(expanded), NULL);
// 		expanded[j] = tmp;
// 		i++;
// 		j++;
// 	}
// 	return (expanded);
// }
