/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:58:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 10:50:24 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_shell(t_shell *shell, int status)
{
	(void)shell;
	exit(status);
}

static bool	is_number(char *str)
{
	int		i;
	int		sign;
	size_t	result;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	if (str[i] < '0' || str[i] > '9')
		return (false);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i++] - '0';
		if ((result > LONG_MAX && sign == 1)
			|| (result > LONG_MAX + (size_t)1 && sign == -1))
			return (false);
	}
	return (str[i] == '\0');
}

int	ft_exit(t_ast *ast, char **args)
{
	unsigned char	i;

	i = 0;
	printf("exit\n");
	if (args[1] == NULL)
		return (clear_shell(ast->shell, 0), 1);
	else if (is_number(args[1]))
	{
		i = ft_atoi(args[1]);
		if (args[2])
			return (printf("minishell: exit: too many arguments\n"), 0);
		clear_shell(ast->shell, i);
	}
	else
	{
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		clear_shell(ast->shell, 255);
	}
	return (0);
}
