/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globber_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:02:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 13:55:46 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_all(char *pattern, char *string);

char	**ft_strsjoin(char **dst, char **src)
{
	int		i;
	int		j;
	char	**new;

	if (!dst || !src)
		return (NULL);
	new = ft_calloc(ft_argslen(dst) + ft_argslen(src) + 2, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (dst[i])
	{
		new[i] = dst[i];
		i++;
	}
	j = 0;
	while (src[j])
	{
		new[i + j] = src[j];
		j++;
	}
	return (new);
}

struct dirent	*ft_readdir(DIR *dir)
{
	struct dirent	*entry;

	errno = 0;
	entry = readdir(dir);
	if (errno)
	{
		ft_putstr_fd("minishell: readdir: ", 2);
		perror("");
		return (NULL);
	}
	return (entry);
}

int	ft_entryjoin(struct dirent *entry, char ***files, char *pattern)
{
	char			**tmp;
	int				i;

	i = ft_argslen(*files);
	if (match_all(pattern, entry->d_name) && ft_strncmp(entry->d_name, ".", 1)
		&& ft_strcmp(entry->d_name, ".."))
	{
		tmp = *files;
		*files = ft_realloc(*files, (i + 1) * sizeof(char *),
				(i + 2) * sizeof(char *));
		if (!*files)
			return (ft_free_args(tmp), ft_putstr_fd(ALLOC_ERR, 2), 1);
		free(tmp);
		(*files)[i++] = ft_strdup(entry->d_name);
		if (!(*files)[i - 1])
			return (ft_free_args(*files), ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	return (0);
}

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

int	match_all(char *pattern, char *string)
{
	while (*string)
	{
		if (match(pattern, string))
			return (1);
		string++;
	}
	return (0);
}
