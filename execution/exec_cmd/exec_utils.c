/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:12:49 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 13:28:45 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_amb_redir(char *file, char **file_globed)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	free(file);
	ft_free_args(file_globed);
	return (NULL);
}

static char	*get_file(char **file_globed, char *skiped)
{
	char	*file;

	if (!*file_globed)
		return (ft_free_args(file_globed), skiped);
	file = ft_strdup(*file_globed);
	if (!file)
		return (ft_free_args(file_globed), free(skiped),
			ft_putstr_fd(ALLOC_ERR, 2), NULL);
	ft_free_args(file_globed);
	free(skiped);
	return (file);
}

char	*ft_file_param(t_ast *ast, char *file)
{
	char	**file_globed;
	char	**exp;
	int		ret;

	ret = 0;
	exp = ft_expand_arg(ast, file);
	if (!exp && ast->command->args)
		return (ft_free_args(exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	else if (ft_argslen(exp) == 0 || ft_argslen(exp) > 1)
		return (handle_amb_redir(file, exp));
	// skip = skip_quotes(exp);
	// free(exp);
	// if (!skip)
	// 	return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	file_globed = ft_calloc(1, sizeof(char *));
	if (!file_globed)
		return (ft_free_args(exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_strchr(exp[0], '*'))
		ret = glob_asterisk(&file_globed, exp[0], is_quoted(file, '*', false));
	if (ret)
		return (ft_free_args(exp), ft_free_args(file_globed), NULL);
	if (!*exp[0] || !is_quoted(file, ' ', true) || ft_argslen(file_globed) > 1)
		return (ft_free_args(exp), handle_amb_redir(exp[0], file_globed));
	return (free(exp), get_file(file_globed, exp[0]));
}
