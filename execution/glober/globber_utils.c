/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globber_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:02:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:07:18 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (ft_strncmp(entry->d_name, ".", 1) != 0 && match(pattern, entry->d_name))
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

static int	ft_check_first_last(char *pattern, char *string, char **strs)
{
	size_t	first_len;
	size_t	last_len;
	size_t	s_len;

	if (!*strs)
		return (1);
	first_len = ft_strlen(strs[0]);
	last_len = ft_strlen(strs[ft_argslen(strs) - 1]);
	s_len = ft_strlen(string);
	if (*pattern != '*' && ft_strncmp(string, strs[0], first_len))
		return (0);
	if (pattern[ft_strlen(pattern) - 1] != '*')
	{
		if (s_len < last_len)
			return (0);
		if (ft_strncmp(string + s_len - last_len, strs[ft_argslen(strs) - 1],
				last_len))
			return (0);
		return (1);
	}
	return (1);
}

int	match(char *pattern, char *string)
{
	char	**strs;
	char	*ptr;
	int		i;
	int		j;

	strs = ft_spliter(pattern, is_not_quoted);
	if (!strs)
		return (0);
	if (ft_check_first_last(pattern, string, strs) == 0)
		return (ft_free_args(strs), 0);
	i = 0;
	j = 0;
	while (strs[i])
	{
		ptr = ft_strstr(string + j, strs[i]);
		if (!ptr)
			return (ft_free_args(strs), 0);
		j = ptr - string + ft_strlen(strs[i]);
		i++;
	}
	ft_free_args(strs);
	return (1);
}
