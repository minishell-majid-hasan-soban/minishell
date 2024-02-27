/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:35:51 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 13:59:26 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_child(t_ast *ast)
{
	int				status;
	pid_t			pid;

	status = 0;
	pid = ft_fork(ast);
	if (pid < 0)
		return (1);
	else if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (exec_redir(ast, ast->command->redirections) == 1)
			exit(1);
		status = exec_args(ast);
		exit(status);
	}
	else
		ft_waitpid(ast, pid, &status);
	return (status);
}

int	exec_parent(t_ast *ast)
{
	int	status;

	status = 0;
	if (exec_redir(ast, ast->command->redirections) == 1)
		return (1);
	status = exec_args(ast);
	return (status);
}

void	exec_child_pipe(t_ast *ast, t_node_dir dir, int fd[2])
{
	signal(SIGQUIT, SIG_DFL);
	if (dir == N_LEFT)
	{
		ft_close(ast, fd[0]);
		ft_dup2(ast, fd[1], 1);
		ft_close(ast, fd[1]);
	}
	else if (dir == N_RIGHT)
	{
		ft_close(ast, fd[1]);
		ft_dup2(ast, fd[0], 0);
		ft_close(ast, fd[0]);
	}
	if (ast->error != T_NONE)
		exit(1);
	exit(exec_ast(ast));
}
