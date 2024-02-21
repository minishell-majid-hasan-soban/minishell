/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:12:49 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 10:35:12 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_file_param(t_ast *ast, char *file)
{
	char	**file_globed;
	char	*file_exp;
	int		ret;

	ret = 0;
	file_exp = ft_expand_arg(ast, file);
	if (!file_exp)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	file_globed = ft_calloc(1, sizeof(char *));
	if (!file_globed)
		return (free(file_exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_strchr(file_exp, '*'))
		ret = glob_asterisk(&file_globed, file_exp);
	if (ret)
		return (free(file_exp), NULL);
	if (!*file_exp || ft_strchr(file_exp, ' ') || ft_argslen(file_globed) > 1)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(file, 2));
		ft_putstr_fd("ambiguous redirect\n", 2);
		return (free(file_exp), ft_free_args(file_globed), NULL);
	}
	if (ft_argslen(file_globed) == 1)
		return (free(file_exp), file_exp = ft_strdup(file_globed[0]),
			ft_free_args(file_globed), file_exp);
	return (ft_free_args(file_globed), file_exp);
}
