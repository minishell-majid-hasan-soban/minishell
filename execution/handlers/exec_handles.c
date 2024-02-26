/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:28:33 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/24 13:47:06 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_exit_status(void)
{
	if (errno == 13 || errno == 20 || errno == 8 || errno == 21)
		exit(126);
	else
		exit(127);
}

void	ft_execve(t_ast *ast, char *path, char **args)
{
	char	**env;

	env = ft_env_to_arr(ast->shell->env);
	if (!env)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(ALLOC_ERR, 2);
		exit(1);
	}
	if (execve(path, args, env) == -1)
	{
		if (errno == 8)
			exit(0);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": ", 2);
		perror("");
		ft_free_args(env);
		ft_exit_status();
	}
}
