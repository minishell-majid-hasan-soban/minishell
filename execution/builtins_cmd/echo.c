/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:42:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/18 10:25:13 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_ast *ast, char **args)
{
	int		option;
	char	*str;

	option = 0;
	if (args && *args)
		args++;
	while (*args && ft_strncmp(*args, "-n", 2) == 0)
	{
		option = 1;
		args++;
	}
	while (*args)
	{
		str = ft_expand_arg(ast, *args);
		if (!str)
			return (ft_putstr_fd("minishell: echo: malloc error\n", 2), 1);
		printf("%s", str);
		free(str);
		args++;
		if (*args)
			printf("%s", " ");
	}
	if (option == 0)
		printf("\n");
	return (0);
}
