/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:09:09 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/23 14:15:27 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_close(t_ast *ast, int fd)
{
	if (fd < 0)
		return ;
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
