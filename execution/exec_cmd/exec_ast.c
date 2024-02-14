/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/14 17:14:52 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_ast(t_ast *ast);

static void exec_cmd(t_ast *ast)
{
	char			**args;
	t_redirection	*redir;

	args = ast->command->expanded_args;
	redir = ast->command->redirections;
	if (exec_redir(ast, redir) == 1)
		exit(1);
	if (args && args[0])
		exec_args(ast, args);
	exit(1);
}

static void exec_child_pipe(t_ast *ast, t_node_dir dir)
{
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
	exec_ast(ast);
}

static void exec_and(t_ast *ast)
{
	exec_ast(ast->left);
	if (ast->shell->exit_status == 0)
		exec_ast(ast->right);
}

static void exec_or(t_ast *ast)
{
	exec_ast(ast->left);
	if (ast->shell->exit_status != 0)
		exec_ast(ast->right);
}

static void	exec_pipe(t_ast *ast)
{
	pid_t	l_pid;
	pid_t	r_pid;

	ft_pipe(ast, ast->command->fd);
	l_pid = ft_fork(ast);
	if (l_pid == 0)
		exec_child_pipe(ast->left
		, N_LEFT);
	else
	{
		r_pid = ft_fork(ast);
		if (r_pid == 0)
			exec_child_pipe(ast, N_RIGHT);
		ft_close_pipe(ast, ast->command->fd);
	}
}

static void	exec_ast(t_ast *ast)
{
	if (ast->type == N_PIPE)
		exec_pipe(ast);
	else if (ast->type == N_AND)
		exec_and(ast);
	else if (ast->type == N_OR)
		exec_or(ast);
	else if (ast->type == N_CMD)
		exec_cmd(ast);
}
