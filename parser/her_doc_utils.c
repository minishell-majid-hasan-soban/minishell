/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   her_doc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:29:52 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 19:26:42 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_dollar(t_ast *ast, char **expanded, char **arg)
{
	char	*var;
	char	*tmp;

	(*arg)++;
	if (**arg == '\0')
		var = ft_strdup("$");
	else if (**arg == '?')
		var = handle_question(ast, arg);
	else
		var = handle_alphanum(ast, arg, true);
	if (var == NULL)
		return ;
	tmp = *expanded;
	*expanded = ft_strjoin(*expanded, var);
	(free(tmp), free(var));
}

static void	ft_not_dollar(char **arg, char **expanded)
{
	char	*to_free;
	char	*tmp;
	int		i;

	i = 0;
	while ((*arg)[i] && (*arg)[i] != '$')
		i++;
	tmp = *expanded;
	to_free = ft_substr(*arg, 0, i);
	*expanded = ft_strjoin(*expanded, to_free);
	(free(tmp), free(to_free));
	*arg += i;
}

char	*expand_heredoc(t_ast *ast, char *line)
{
	char	*expanded;

	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	while (*line)
	{
		if (*line == '$')
			ft_dollar(ast, &expanded, &line);
		else
			ft_not_dollar(&line, &expanded);
	}
	return (expanded);
}
