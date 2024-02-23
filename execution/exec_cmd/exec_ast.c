/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/23 08:14:26 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_and(t_ast *ast)
{
	int	status;

	status = exec_ast(ast->left);
	if (status == 0)
		status = exec_ast(ast->right);
	return (status);
}

static int	exec_or(t_ast *ast)
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
	int		fd[2];

	ft_pipe(ast, fd);
	l_pid = ft_fork(ast);
	if (l_pid < 0)
		return (ft_close_pipe(ast, fd), 1);
	else if (l_pid == 0)
		exec_child_pipe(ast->left, N_LEFT, fd);
	r_pid = ft_fork(ast);
	if (r_pid < 0)
		return (ft_close_pipe(ast, fd), 1);
	else if (r_pid == 0)
		exec_child_pipe(ast->right, N_RIGHT, fd);
	ft_close_pipe(ast, fd);
	ft_waitpid(ast, l_pid, &status);
	ft_waitpid(ast, r_pid, &status);
	if (ast->error != T_NONE)
		return (1);
	return (status);
}

static int	exec_cmd(t_ast *ast)
{
	int				status;
	char			**args;

	status = 0;
	args = ft_expand_args(ast, ast->command->args);
	ast->command->expanded_args = args;
	ast->command->skiped_args = ft_skip_args(args);
	ast->command->globed_args = ft_glob_args(ast, ast->command->skiped_args);
	if (!args || !args[0] || is_builtin(args[0]) == 1)
		status = exec_parent(ast);
	else
		status = exec_child(ast);
	return (status);
}

int	exec_ast(t_ast *ast)
{
	if (exec_redir(ast, ast->redirections) == 1)
		return (1);
	if (ast->type == N_PIPE)
		return (exec_pipe(ast));
	else if (ast->type == N_AND)
		return (exec_and(ast));
	else if (ast->type == N_OR)
		return (exec_or(ast));
	else if (ast->type == N_CMD)
		return (exec_cmd(ast));
	return (0);
}
