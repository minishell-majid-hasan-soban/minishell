/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:28:33 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 18:28:50 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_status(void)
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
		ft_putstr_fd(": malloc: failed to allocate memory\n", 2);
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
		exit_status();
	}
}

void	ft_close(t_ast *ast, int fd)
{
	if (close(fd) == -1)
	{
		ft_putstr_fd("minishell: close: ", 2);
		perror("");
		if (ast)
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
	if (dup2(old_fd, new_fd) == -1)
	{
		if (ast && ast->error == T_NONE)
			return ;
		ast->error = T_FUNC;
		ft_putstr_fd("minishell: dup2: ", 2);
		perror("");
	}
}

pid_t	ft_fork(t_ast *ast)
{
	pid_t	pid;

	if (ast && ast->error != T_NONE)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		ast->error = T_FUNC;
		ft_putstr_fd("minishell: fork: ", 2);
		perror("");
	}
	return (pid);
}

void	ft_pipe(t_ast *ast, int fd[2])
{
	if (pipe(fd) == -1)
	{
		if (ast && ast->error != T_NONE)
			return ;
		ast->error = T_FUNC;
		ft_putstr_fd("minishell: pipe: ", 2);
		perror("");
	}
}
