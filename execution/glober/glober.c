/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glober.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:16:55 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 18:56:59 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (*pattern == '\0')
				return (1);
			while (*string && *string != *pattern)
				string++;
		}
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0' && *string == '\0');
}

static char	**glob_arg(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**files;
	char			**tmp;
	int				i;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	files = ft_calloc(1, sizeof(char *));
	if (!files)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (match(pattern, entry->d_name) && ft_strncmp(entry->d_name, ".", 1)
			&& ft_strcmp(entry->d_name, ".."))
		{
			tmp = files;
			files = ft_realloc(files, (i + 1) * sizeof(char *),
					(i + 2) * sizeof(char *));
			if (!files)
				return (ft_free_args(tmp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
			free(tmp);
			files[i++] = ft_strdup(entry->d_name);
			if (!files[i - 1])
				return (ft_free_args(files), ft_putstr_fd(ALLOC_ERR, 2), NULL);
		}
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		return (ft_putstr_fd("minishell: closedir error\n", 2), files);
	return (files);
}

static char	**ft_strsjoin(char **dst, char **src)
{
	int		i;
	int		j;
	char	**new;

	if (!src || !*src)
		return (NULL);
	new = ft_calloc(ft_argslen(dst) + ft_argslen(src) + 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = -1;
	while (dst[++i])
		new[i] = dst[i];
	j = -1;
	while (src[++j])
		new[i + j] = src[j];
	return (new);
}

static int	handle_no_asterisk(char ***globed_args, char args)
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

static int	handle_asterisk(char ***globed_args, char *args)
{
	char	**new_args;
	char	**tmp;
	int		i;

	new_args = glob_arg(args);
	if (!new_args)
		return (ft_free_args(*globed_args), ft_putstr_fd(ALLOC_ERR, 2), 1);
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
	char	**new_args;
	int		ret;

	(void)ast;
	if (!args)
		return (NULL);
	i = 0;
	ret = 0;
	new_args = ft_calloc(1, sizeof(char *));
	globed_args = ft_calloc(1, sizeof(char *));
	while (args[i])
	{
		if (ft_strchr(args[i], '*') == NULL)
			ret = handle_no_asterisk(&globed_args, args[i]);
		else
			ret = handle_asterisk(&globed_args, args[i]);
		if (ret)
			return (NULL);
		i++;
	}
	return (globed_args);
}
