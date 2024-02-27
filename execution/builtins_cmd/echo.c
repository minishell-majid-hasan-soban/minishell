/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:42:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 13:51:59 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_flag(char *arg)
{
	if (*arg == '-')
	{
		arg++;
		while (*arg == 'n')
			arg++;
		if (*arg == '\0')
			return (1);
	}
	return (0);
}

int	ft_echo(t_ast *ast, char **args)
{
	int		option;

	(void)ast;
	option = 0;
	if (*args)
		args++;
	while (*args)
	{
		if (is_flag(*args))
			option = 1;
		else
			break ;
		args++;
	}
	while (*args)
	{
		printf("%s", *args);
		args++;
		if (*args)
			printf("%s", " ");
	}
	if (option == 0)
		printf("\n");
	return (0);
}
