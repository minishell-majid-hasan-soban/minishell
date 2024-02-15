/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/15 10:46:06 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_ast(t_ast *ast);

static int	exec_child(t_ast *ast, char **args, t_redirection *redir)
{
	int				status;
	pid_t			pid;
	
	status = 0;
	pid = ft_fork(ast);
	if (pid < 0)
		return (1);
	else if (pid == 0)
	{
		if (exec_redir(ast, redir) == 1 || ast->error != T_NONE)
			exit(1);
		status = exec_args(ast, args);
		exit(status);
	}
	else
		ft_waitpid(ast, pid, &status);
	if (args && args[0])
		status = exec_args(ast, args);
	return (1);
}

static int exec_cmd(t_ast *ast)
{
	int				status;
	t_redirection	*redir;
	char			**args;

	status = 0;
	args = ast->command->expanded_args;
	redir = ast->command->redirections;
	if (!args || !args[0] || is_builtin(ast, args) == 1)
		status = exec_parent(ast, args, redir);
	else
		status = exec_child(ast, args, redir);
	
	return (ast->error != T_NONE);
}

static void exec_child_pipe(t_ast *ast, t_node_dir dir)
{
	int	status;
	
	if (dir == N_LEFT)
	{
		ft_close(ast, ast->command->fd[0]);
		ft_dup2(ast, ast->command->fd[1], 1);
		ft_close(ast, ast->command->fd[1]);
	}
	else if (dir == N_RIGHT)
	{
		ft_close(ast, ast->command->fd[1]);
		ft_dup2(ast, ast->command->fd[0], 0);
		ft_close(ast, ast->command->fd[0]);
	}
	if (ast->error != T_NONE)
		exit(1);
	exit(exec_ast(ast));
}

static int exec_and(t_ast *ast)
{
	int	status;
	
	status = exec_ast(ast->left);
	if (status == 0)
		status = exec_ast(ast->right);
	return (status);
}

static int exec_or(t_ast *ast)
{
	int	status;

	status = exec_ast(ast->left);
	if (status != 0)
		status = exec_ast(ast->right);
	return (status);
}

static int	exec_pipe(t_ast *ast)
{
	pid_t	l_pid;
	pid_t	r_pid;
	int		status;

	ft_pipe(ast, ast->command->fd);
	l_pid = ft_fork(ast);
	if (l_pid < 0)
		return (ft_close_pipe(ast, ast->command->fd), 1);
	else if (l_pid == 0)
		exec_child_pipe(ast->left, N_LEFT);
	else
	{
		r_pid = ft_fork(ast);
		if (r_pid < 0)
			return (ft_close_pipe(ast, ast->command->fd), 1);
		else if (r_pid == 0)
			exec_child_pipe(ast, N_RIGHT);
		ft_close_pipe(ast, ast->command->fd);
		ft_waitpid(ast, l_pid, &status);
		ft_waitpid(ast, r_pid, &status);
		if (ast->error != T_NONE)
			return (1);
		return (status);
	}
}

static int	exec_ast(t_ast *ast)
{
	if (ast->type == N_PIPE)
		return (exec_pipe(ast));
	else if (ast->type == N_AND)
		return (exec_and(ast));
	else if (ast->type == N_OR)
		return (exec_or(ast));
	else if (ast->type == N_CMD)
		return (exec_cmd(ast));
}
