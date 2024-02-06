/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:37:07 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 13:55:10 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*str;
// 	int		i;
// 	int		k;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
// 	if (str == NULL)
// 		return (NULL);
// 	i = 0;
// 	k = 0;
// 	while (s1[i])
// 		str[k++] = s1[i++];
// 	i = 0;
// 	while (s2[i])
// 		str[k++] = s2[i++];
// 	str[k] = '\0';
// 	return (str);
// }

// size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
// {
// 	size_t	lensrc;
// 	size_t	i;

// 	lensrc = ft_strlen(src);
// 	if (dstsize == 0)
// 		return (lensrc);
// 	i = 0;
// 	while (i < dstsize - 1 && src[i])
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	dest[i] = 0;
// 	return (lensrc);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	slen;
// 	char	*str;

// 	if (!s)
// 		return (0);
// 	slen = ft_strlen(s);
// 	if (start > slen)
// 		len = 0;
// 	else if (len > slen - start)
// 		len = slen - start;
// 	str = (char *)malloc((len + 1) * sizeof(char));
// 	if (!str)
// 		return (NULL);
// 	start *= (start <= slen);
// 	ft_strlcpy(str, s + start, len + 1);
// 	return (str);
// }
