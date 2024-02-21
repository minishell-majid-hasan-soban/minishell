/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glober.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:16:55 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 10:05:36 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**glob_arg(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**files;

	dir = opendir(".");
	if (!dir)
		return (ft_putstr_fd("minishell: opendir: ", 2), perror(""), NULL);
	files = ft_calloc(1, sizeof(char *));
	if (!files)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	entry = ft_readdir(dir);
	while (entry)
	{
		if (ft_entryjoin(entry, &files, pattern))
			return (NULL);
		entry = ft_readdir(dir);
	}
	if (closedir(dir) == -1)
		return (ft_putstr_fd("minishell: closedir error\n", 2), files);
	return (files);
}

static int	handle_no_asterisk(char ***globed_args, char *args)
{
	char	**tmp;

	tmp = *globed_args;
	*globed_args = ft_realloc((*globed_args),
			(ft_argslen(*globed_args) + 1) * sizeof(char *),
			(ft_argslen(*globed_args) + 2) * sizeof(char *));
	if (!(*globed_args))
		return (ft_free_args(tmp), ft_putstr_fd(ALLOC_ERR, 2), 1);
	free(tmp);
	(*globed_args)[ft_argslen(*globed_args)] = ft_strdup(args);
	return (0);
}

int	glob_asterisk(char ***globed_args, char *args)
{
	char	**new_args;
	char	**tmp;

	new_args = glob_arg(args);
	if (!new_args)
		return (ft_free_args(*globed_args), *globed_args = NULL,
			ft_putstr_fd(ALLOC_ERR, 2), 1);
	tmp = *globed_args;
	(*globed_args) = ft_strsjoin(*globed_args, new_args);
	if (!(*globed_args))
		return (ft_free_args(tmp), ft_free_args(new_args),
			ft_putstr_fd(ALLOC_ERR, 2), 1);
	free(tmp);
	free(new_args);
	return (0);
}

char	**ft_glob_args(t_ast *ast, char **args)
{
	int		i;
	char	**globed_args;
	int		ret;

	(void)ast;
	if (!args)
		return (NULL);
	i = 0;
	ret = 0;
	globed_args = ft_calloc(1, sizeof(char *));
	while (args[i])
	{
		if (ft_strchr(args[i], '*') == NULL)
			ret = handle_no_asterisk(&globed_args, args[i]);
		else
			ret = glob_asterisk(&globed_args, args[i]);
		if (ret)
			return (NULL);
		i++;
	}
	return (globed_args);
}
