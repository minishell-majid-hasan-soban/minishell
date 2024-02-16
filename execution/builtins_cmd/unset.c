/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:23:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/16 07:53:10 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unsetenv(char *name, t_shell *shell)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = shell->env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (prev == NULL)
				shell->env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_ast *ast, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		ft_unsetenv(args[i], ast->shell);
		i++;
	}
	return (0);
}
