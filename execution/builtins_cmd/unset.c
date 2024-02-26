/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:23:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 14:27:29 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unsetenv(char *name, t_shell *shell)
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
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_unset(t_ast *ast, char **args)
{
	int	i;
<<<<<<< HEAD
	int	status;
=======
	int	ret;
>>>>>>> 2ae027bc8ca0e0aea4837bc38b1159c703eb2821

	ret = 0;
	i = 1;
	while (args[i])
	{
<<<<<<< HEAD
		status = ft_unsetenv(args[i], ast->shell);
		i++;
	}
	return (status);
=======
		if (!ft_valid_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			ft_unsetenv(args[i], ast->shell);
		i++;
	}
	return (ret);
>>>>>>> 2ae027bc8ca0e0aea4837bc38b1159c703eb2821
}
