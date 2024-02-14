/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:28:33 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/14 18:20:19 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_execve(t_ast *ast, char **args)
{
	char	**env;

	env = ft_env_to_arr(ast->shell->env);
	if (!env)
		return (ast->error = T_FUNC, 1);
	if (execve(args[0], args, env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": ", 2);
		perror("");
		ft_free_args(env);
		return (1);
	}
	return (0);
}

void	ft_close(t_ast *ast, int fd)
{
	if (close(fd) == -1)
	{
		perror(ast->shell->name);
		ast->error = T_FUNC;
	}
}

void	ft_close_pipe(t_ast *ast, int fd[2])
{
	ft_close(ast, fd[0]);
	ft_close(ast, fd[1]);
}

void	ft_dup2(t_ast *ast, int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1 && ast->error == T_NONE)
	{
		ast->error = T_FUNC;
		perror(ast->shell->name);
	}
}

pid_t	ft_fork(t_ast *ast)
{
	pid_t	pid;

	if (ast->error != T_NONE)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		ast->error = T_FUNC;
		perror(ast->shell->name);
	}
	return (pid);
}

void	ft_pipe(t_ast *ast, int fd[2])
{
	if (pipe(fd) == -1 && ast->error == T_NONE)
	{
		ast->error = T_FUNC;
		perror(ast->shell->name);
	}
}
