/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 02:22:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 02:22:41 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	count_words(const char *s1, char sep)
{
	size_t	i;

	i = 0;
	while (*s1)
	{
		while (*s1 == sep)
			s1++;
		if (*s1)
			i++;
		while (*s1 && *s1 != sep)
			s1++;
	}
	return (i);
}

static char	*get_word(char const **s1, char sep)
{
	char	*new_str;
	size_t	len;

	while (**s1 && **s1 == sep)
		(*s1)++;
	len = 0;
	while (s1[0][len] && s1[0][len] != sep)
		len++;
	new_str = (char *)malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (0);
	ft_strlcpy(new_str, *s1, len + 1);
	*s1 += len;
	return (new_str);
}

static void	clear(char **strs, size_t size)
{
	size_t	i;

	i = 0;
	while (i <= size)
	{
		if (strs[i])
			free(strs[i]);
		i++;
	}
}

char	**ft_split(char const *s, char c)
{
	size_t	wc;
	size_t	i;
	char	**strs;

	if (!s)
		return (0);
	wc = count_words(s, c);
	strs = (char **)malloc((wc + 1) * sizeof(char *));
	i = 0;
	if (!strs)
		return (0);
	while (i < wc)
	{
		strs[i] = get_word(&s, c);
		if (!strs[i])
		{
			clear(strs, i);
			free(strs);
			return (0);
		}
		i++;
	}
	strs[i] = 0;
	return (strs);
}