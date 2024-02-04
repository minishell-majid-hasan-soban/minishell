/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:42:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 03:07:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char **args)
{
	int	option;

	option = 0;
	while (ft_strcmp("-n", *args) == 0)
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
	if (option == 1)
		printf("\n");
	return (0);
}
