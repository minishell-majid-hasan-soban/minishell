/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:42:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 14:38:59 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_ast *ast, char **args)
{
	int		option;

	(void)ast;
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
		printf("%s", *args);
		args++;
		if (*args)
			printf("%s", " ");
	}
	if (option == 0)
		printf("\n");
	return (0);
}
