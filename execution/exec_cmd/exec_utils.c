/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:12:49 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 08:43:23 by hsobane          ###   ########.fr       */
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
	char	*exp;
	char	*skip;
	int		ret;

	ret = 0;
	exp = ft_expand_arg(ast, file);
	skip = skip_quotes(exp);
	free(exp);
	if (!skip)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	file_globed = ft_calloc(1, sizeof(char *));
	if (!file_globed)
		return (free(skip), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_strchr(skip, '*'))
		ret = glob_asterisk(&file_globed, skip, is_quoted(file, '*', false));
	if (ret)
		return (free(skip), ft_free_args(file_globed), NULL);
	if (!*skip || !is_quoted(file, ' ', true) || ft_argslen(file_globed) > 1)
		return (handle_amb_redir(skip, file_globed));
	return (get_file(file_globed, skip));
}
