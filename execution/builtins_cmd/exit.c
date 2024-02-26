/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:58:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 13:40:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (args[1] == NULL)
		return (ft_putstr_fd("exit\n", 1), ft_free_shell(ast->shell), 1);
	else if (is_number(args[1]))
	{
		i = ft_atoi(args[1]);
		if (args[2])
			return (ft_putstr_fd("minishell: exit: ", 2),
				ft_putstr_fd("too many arguments\n", 2), 1);
		ft_putstr_fd("exit\n", 1);
		ast->shell->exit_status = i;
		ft_free_shell(ast->shell);
	}
	else
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ast->shell->exit_status = 255;
		ft_free_shell(ast->shell);
	}
	return (0);
}
