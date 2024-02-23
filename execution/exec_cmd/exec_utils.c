/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:12:49 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/23 11:23:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_file_param(t_ast *ast, char *file)
{
	char	**file_globed;
	char	*file_exp;
	char	*file_skip;
	int		ret;

	ret = 0;
	file_exp = ft_expand_arg(ast, file);
	file_skip = skip_quotes(file_exp);
	if (!file_skip)
		return (free(file_exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	file_globed = ft_calloc(1, sizeof(char *));
	if (!file_globed)
		return (free(file_skip), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_strchr(file_skip, '*'))
		ret = glob_asterisk(&file_globed, file_skip, is_quoted(file, '*', false));
	if (ret)
		return (free(file_skip), ft_free_args(file_globed), NULL);
	if (!*file_skip || !is_quoted(file, ' ', true) || ft_argslen(file_globed) > 1)
	{
		printf("file: %s\n", file);
		printf("file_exp: %s\n", file_exp);
		printf("file_skip: %s\n", file_skip);
		printf("file_globed: %ld\n", ft_argslen(file_globed));
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2));
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (free(file_skip), ft_free_args(file_globed), NULL);
	}
	if (ft_argslen(file_globed) == 1)
		return (free(file_skip), file_skip = ft_strdup(file_globed[0]),
			ft_free_args(file_globed), file_skip);
	return (ft_free_args(file_globed), file_skip);
}
