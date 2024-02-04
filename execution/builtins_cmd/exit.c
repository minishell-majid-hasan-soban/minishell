/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:58:02 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 07:36:17 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clear_data(t_data *data)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = data->env;
	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

void	ft_exit(t_data *data)
{
	clear_data(data);
	clear_history();
	printf("exit\n");
	exit(0);
}
