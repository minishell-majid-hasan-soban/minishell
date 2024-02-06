/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 12:41:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_ast(t_ast *ast);

static char *ft_get_path(t_ast *ast, char *cmd)
{
	char	**paths;
	char	*path;
	char	*to_free;
	int		i;
	t_env	*env;

	i = 0;
	env = ft_getenv(ast->shell->env, "PATH");
	if (!env)
		return (NULL);
	paths = ft_split(env->value, ':');
	while (paths[i])
	{
		to_free = ft_strjoin(paths[i], "/");
		path = ft_strjoin(to_free, cmd);
		free(to_free);
		if (access(path, X_OK) == 0)
		{
			// ft_free_args(paths);
			return (path);
		}
		// ft_strdel(&path);
		i++;
	}
	// ft_free_args(paths);
	if (access(cmd, X_OK) == 0 && (cmd[0] == '.' || cmd[0] == '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static void exec_cmd(t_ast *ast)
{
	char	**args;
	char	*path;

	args = ast->command->expanded_args;
	path = ft_get_path(ast, args[0]);
	if (path)
	{
		ft_execve(ast, args);
		// ft_strdel(&path);
	}
	else
		ast->shell->exit_status = 127;
	// ft_free_args(args);
	exit(1);
}

static void exec_child_pipe(t_ast *ast)
{
	if (ast->direction == N_LEFT)
	{
		ft_close(ast, ast->command->fd[0]);
		ft_dup2(ast, ast->command->fd[1], 1);
		ft_close(ast, ast->command->fd[1]);
		exec_cmd(ast);
	}
	else if (ast->direction == N_RIGHT)
	{
		ft_close(ast, ast->command->fd[1]);
		ft_dup2(ast, ast->command->fd[0], 0);
		ft_close(ast, ast->command->fd[0]);
		if (ast->type == N_PIPE)
			exec_cmd(ast);
		else
			exec_cmd(ast);
	}
}

static void exec_and(t_ast *ast)
{
	exec_ast(ast);
	if (ast->shell->exit_status == 0)
		exec_ast(ast);
}

static void exec_or(t_ast *ast)
{
	exec_ast(ast);
	if (ast->shell->exit_status != 0)
		exec_ast(ast);
}

static void	exec_pipe(t_ast *ast)
{
	pid_t	l_pid;
	pid_t	r_pid;

	ft_pipe(ast, ast->command->fd);
	l_pid = ft_fork(ast);
	if (l_pid == 0)
		exec_child_pipe(ast);
	else
	{
		r_pid = ft_fork(ast);
		if (r_pid == 0)
			exec_child_pipe(ast);
		ft_close_pipe(ast, ast->command->fd);
	}
}

static void	exec_ast(t_ast *ast)
{
	if (ast->type == N_PIPE)
		exec_pipe(ast);
	else if (ast->type == N_CMD)
		exec_cmd(ast);
	else if (ast->type == N_AND)
		exec_and(ast);
	else if (ast->type == N_OR)
		exec_or(ast);
}
