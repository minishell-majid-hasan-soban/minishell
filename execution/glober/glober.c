/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glober.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:16:55 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 18:33:30 by hsobane          ###   ########.fr       */
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
	files = ft_calloc(2, sizeof(char *));
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
	if (files[0])
		return (files);
	files[0] = ft_strdup(pattern);
	if (!files[0])
		return (free(files), ft_putstr_fd(ALLOC_ERR, 2), NULL);
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

int	glob_asterisk(char ***globed_args, char *args, bool quoted)
{
	char	**new_args;
	char	**tmp;

	if (!quoted)
		new_args = glob_arg(args);
	else
	{
		new_args = ft_calloc(2, sizeof(char *));
		if (!new_args)
			return (ft_putstr_fd(ALLOC_ERR, 2), ft_free_args(*globed_args),
				*globed_args = NULL, 1);
		new_args[0] = ft_strdup(args);
		if (!new_args[0])
			return (free(new_args), ft_putstr_fd(ALLOC_ERR, 2),
				ft_free_args(*globed_args), *globed_args = NULL, 1);
	}
	if (!new_args)
		return (ft_free_args(*globed_args), *globed_args = NULL, 1);
	tmp = *globed_args;
	(*globed_args) = ft_strsjoin(*globed_args, new_args);
	if (!(*globed_args))
		return (ft_free_args(tmp), ft_free_args(new_args),
			ft_putstr_fd(ALLOC_ERR, 2), 1);
	free(tmp);
	free(new_args);
	return (0);
}

bool	is_quoted(char *arg)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			if (quote == 0)
				quote = arg[i];
			else if (quote == arg[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
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
			ret = glob_asterisk(&globed_args, args[i], false);
		if (ret)
			return (NULL);
		i++;
	}
	return (globed_args);
}
