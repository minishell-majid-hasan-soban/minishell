/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glober.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:16:55 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/19 18:41:34 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int match(char *pattern, char *string)
{
    while (*pattern && *string)
	{
        if (*pattern == '*')
		{
            while (*pattern == '*')
                pattern++;
            if (*pattern == '\0')
                return 1;
            while (*string && *string != *pattern)
                string++;
        } 
		else if (*pattern == *string)
		{
            pattern++;
            string++;
        }
		else
            return 0;
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
	int				i;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	files = ft_calloc(1, sizeof(char *));
	if (!files)
		return (ft_putstr_fd(MALLOC_ERROR, 2),  NULL);
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (match(pattern, entry->d_name))
		{
			files = ft_realloc(files, (i + 1) * sizeof(char *), (i + 2) * sizeof(char *));
			files[i++] = ft_strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		return (NULL);
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

char	**ft_glober(t_ast *ast, char **args)
{
	int		i;
	char	**globed_args;
	char	**new_args;
	char	**tmp;

	i = 0;
	new_args = ft_calloc(1, sizeof(char *));
	globed_args = ft_calloc(1, sizeof(char *));
	while (args[i])
	{
		new_args = glob_arg(args[i]);
		tmp = globed_args;
		globed_args = ft_strsjoin(&globed_args, new_args);
		(free(tmp), free(new_args));
		i++;
	}
}
