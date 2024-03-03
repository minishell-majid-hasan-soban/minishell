/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 03:04:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 09:11:17 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

size_t	ft_argslen(char **args)
{
	size_t	len;

	len = 0;
	while (args && args[len])
		len++;
	return (len);
}

static size_t	count_words(const char *s1, bool (*func)(char *, int))
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (s1[j])
	{
		while (func((char *)s1, j))
			j++;
		if (s1[j])
			i++;
		while (s1[j] && !func((char *)s1, j))
			j++;
	}
	return (i);
}

static char	*get_word(char const **s1, bool (*func)(char *, int))
{
	char	*new_str;
	size_t	len;
	int		i;

	i = 0;
	while ((*s1)[i] && func((char *)*s1, i))
		i++;
	len = 0;
	while ((*s1)[len + i] && !func((char *)*s1, len + i))
		len++;
	new_str = (char *)malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (0);
	ft_strlcpy(new_str, &((*s1)[i]), len + 1);
	*s1 += len + i;
	return (new_str);
}

char	**ft_spliter(char const *s, bool (*func)(char *, int))
{
	size_t	wc;
	size_t	i;
	char	**strs;

	if (!s)
		return (0);
	wc = count_words(s, func);
	strs = (char **)ft_calloc(wc + 1, sizeof(char *));
	i = 0;
	if (!strs)
		return (0);
	while (i < wc)
	{
		strs[i] = get_word(&s, func);
		if (!strs[i])
			return (ft_putstr_fd(ALLOC_ERR, 2), ft_free_args(strs), (void *)0);
		i++;
	}
	strs[i] = 0;
	return (strs);
}
