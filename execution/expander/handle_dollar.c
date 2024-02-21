/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:16:12 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 14:13:43 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_undescore(t_ast *ast, char **arg)
{
	char	*var;

	(*arg)++;
	var = get_value(ast->shell->env, "_");
	return (var);
}

static char	*handle_question(t_ast *ast, char **arg)
{
	char	*var;

	(*arg)++;
	var = ft_itoa(ast->shell->exit_status);
	return (var);
}

static char	*handle_alphanum(t_ast *ast, char **arg, bool quoted)
{
	char	*var;
	char	*to_free;
	int		i;

	i = 0;
	while ((*arg)[i] && (ft_isalnum((*arg)[i]) || (*arg)[i] == '_'))
		i++;
	if (i == 0 && !quoted)
		return (ft_strdup(""));
	else if (i == 0)
		return (ft_strdup("$"));
	to_free = ft_substr(*arg, 0, i);
	var = get_value(ast->shell->env, to_free);
	free(to_free);
	*arg += i;
	return (var);
}

void	handle_dollar(t_ast *ast, char **arg, char **expanded, bool quoted)
{
	char	*var;
	char	*to_free;

	(*arg)++;
	if (**arg == '\0')
		var = ft_strdup("$");
	else if (**arg == '?')
		var = handle_question(ast, arg);
	else if (**arg == '_')
		var = handle_undescore(ast, arg);
	else
		var = handle_alphanum(ast, arg, quoted);
	to_free = *expanded;
	*expanded = ft_strjoin(*expanded, var);
	(free(to_free), free(var));
}
