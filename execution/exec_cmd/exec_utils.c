/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:12:49 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 08:18:27 by hsobane          ###   ########.fr       */
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

char	*ft_file_param(t_ast *ast, char *file)
{
	char	**file_globed;
	char	*exp;
	char	*skip;
	int		ret;

	ret = 0;
	exp = ft_expand_arg(ast, file);
	skip = skip_quotes(exp);
	if (!skip)
		return (free(exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	file_globed = ft_calloc(1, sizeof(char *));
	if (!file_globed)
		return (free(skip), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_strchr(skip, '*'))
		ret = glob_asterisk(&file_globed, skip, is_quoted(file, '*', false));
	if (ret)
		return (free(skip), ft_free_args(file_globed), NULL);
	if (!*skip || !is_quoted(file, ' ', true) || ft_argslen(file_globed) > 1)
		return (handle_amb_redir(file, file_globed));
	if (ft_argslen(file_globed) == 1)
		return (free(skip), skip = ft_strdup(file_globed[0]),
			ft_free_args(file_globed), skip);
	return (ft_free_args(file_globed), skip);
}
