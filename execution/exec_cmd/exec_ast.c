/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 16:56:34 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_child(t_ast *ast)
{
	int				status;
	pid_t			pid;
	
	status = 0;
	pid = ft_fork(ast);
	if (pid < 0)
		return (1);
	else if (pid == 0)
	{
		if (exec_redir(ast) == 1)
			exit(1);
		status = exec_args(ast);
		exit(status);
	}
	else
		ft_waitpid(ast, pid, &status);
	return (status);
}

static int	exec_parent(t_ast *ast)
{
	int	status;
	
	status = 0;
	if (exec_redir(ast) == 1)
		return (1);
	fflush(stdout);
	status = exec_args(ast);
	return (status);
}

void print_args(char **args, char *name)
{
	int	i;

	i = 0;
	printf("Command: %s\n", name);
	while (args[i])
	{
		printf("args[%d]: %s\n", i, args[i]);
		i++;
	}
	printf("\n");
}

static int exec_cmd(t_ast *ast)
{
	int				status;
	char			**args;

	status = 0;
	args = ft_expand_args(ast, ast->command->args);
	print_args(args, "expanded_args");
	ast->command->expanded_args = args;
	ast->command->globed_args = ft_glob_args(ast, args);
	print_args(ast->command->globed_args, "globed_args");
	// print_args(globed_args, "globed_args");
	// print_args(args, "expanded_args");
	if (!args || !args[0] || is_builtin(args[0]) == 1)
		status = exec_parent(ast);
	else
		status = exec_child(ast);
	return (status);
}

static void exec_child_pipe(t_ast *ast, t_node_dir dir, int fd[2])
{
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

int	exec_ast(t_ast *ast)
{
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
