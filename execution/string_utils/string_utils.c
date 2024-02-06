/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 03:04:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 13:54:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		ft_strlen(const char *s)
// {
// 	int		i;

// 	i = 0;
// 	while (s && s[i])
// 		i++;
// 	return (i);
// }

int		ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s1 == *s2)
		s1++ && s2++;
	return (*s1 - *s2);
}

// char	*ft_strdup(const char *s1)
// {
// 	char	*str;
// 	int		i;

// 	i = ft_strlen(s1);
// 	str = (char *)malloc(sizeof(char) * (i + 1));
// 	if (str == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		str[i] = s1[i];
// 		i++;
// 	}
// 	str[i] = 0;
// 	return (str);
// }

// int	ft_putstr_fd(char *s, int fd)
// {
// 	int		i;

// 	i = 0;
// 	while (s && s[i])
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// 	return (i);
// }

// char	*ft_strchr(const char *s, int c)
// {
// 	char	*str;

// 	str = (char *)s;
// 	while (str && *str && *str != (char)c)
// 		str++;
// 	if (*str == (char)c)
// 		return (str);
// 	return (NULL);
// }
