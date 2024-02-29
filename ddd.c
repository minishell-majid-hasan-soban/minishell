/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 10:16:50 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 14:03:28 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/libft.h"
# include <libc.h>

// ((ls < a | cat > b) < d < e && (cat >> t >> e) < a < b << r || echo fffff ) < a << e

int	is_in_str(char *str, char c)
{
	while (*str && *str != c)
		str++;
	return (*str == c);
}

int	skip_quotes_work(char **dest, char **str, char quote)
{
	(*str)++;
	// if (!is_in_str(*str, quote))
	// {
	// 	ft_putstr_fd("minishell: syntax error, expected `", 2);
	// 	ft_putchar_fd(quote, 2);
	// 	ft_putstr_fd("`\n", 2);
	// 	return (-1);
	// }
	while (**str && **str != quote)
	{
		*(*dest) = *(*str);
		(*str)++;
		(*dest)++;
	}
	(*str)++;
	return (1);
}

static	int	handle_quotes(char *dest, char *str, int *i, int *j)
{
	char	*tmp_str;
	char	*tmp_dest;

	tmp_str = str + *i;
	tmp_dest = dest + *j;
	if (skip_quotes_work(&tmp_dest, &tmp_str, str[*i]))
	{
		*i += tmp_str - (str + *i);
		*j += tmp_dest - (dest + *j);
	}
	else
		return (-1);
	return (1);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\f' || c == '\r' || c == '\v')
		return (1);
	return (0);
}

char	*skip_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (ft_putstr_fd("minishell: malloc: ", 2), perror(""), NULL);
	i = 0;
	j = 0;
	while (str[i] && !is_space(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
			handle_quotes(result, str, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = 0;
	return (result);
}

int main(int ac, char **av)
{
	if (av && av[1])
	{
		printf("%s\n", av[1]);		
		printf("%s\n", skip_quotes(av[1]));
	}
}