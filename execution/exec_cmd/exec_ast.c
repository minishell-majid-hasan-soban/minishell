/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:22:54 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/04 14:34:36 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_get_path(t_shell *shell, char *cmd)
{
	char	**paths;
	char	*path;
	char	*to_free;
	int		i;

	i = 0;
	paths = ft_plit(ft_getenv(shell->env, "PATH"), ':');
	while (paths[i])
	{
		to_free = ft_strjoin(paths[i], "/");
		path = ft_strjoin(to_free, cmd);
		free(to_free);
		if (access(path, X_OK) == 0)
		{
			ft_free_args(paths);
			return (path);
		}
		ft_strdel(&path);
		i++;
	}
	ft_free_args(paths);
	if (access(cmd, X_OK) == 0 && (cmd[0] == '.' || cmd[0] == '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static void exec_cmd(t_shell *shell, t_ast *ast)
{
	char	**args;
	char	*path;

	args = ast->command->expanded_args;
	path = ft_get_path(shell, args[0]);
	if (path)
	{
		ft_execve(shell, path, args, shell->env);
		ft_strdel(&path);
	}
	else
		shell->exit_status = 127;
	ft_free_args(args);
	exit(1);
}

static void exec_child_pipe(t_shell *shell, t_ast *ast)
{
	if (ast->direction == N_LEFT)
	{
		ft_close(shell, shell->p_fd[0]);
		ft_dup2(shell, shell->p_fd[1], 1);
		ft_close(shell, shell->p_fd[1]);
		exec_cmd(shell, ast);
	}
	else if (ast->direction == N_RIGHT)
	{
		ft_close(shell, shell->p_fd[1]);
		ft_dup2(shell, shell->p_fd[0], 0);
		ft_close(shell, shell->p_fd[0]);
		if (ast->type == N_PIPE)
			exec_cmd(shell, ast->left);
		else
			exec_cmd(shell, ast);
	}
}

static void exec_and(t_shell *shell, t_ast *ast)
{
	exec_ast(shell);
	if (shell->exit_status == 0)
		exec_ast(shell);
}

static void	exec_pipe(t_shell *shell, t_ast *ast)
{
	pid_t	l_pid;
	pid_t	r_pid;
	int		status;

	ft_pipe(shell);
	l_pid = ft_fork(shell);
	if (l_pid == 0)
		exec_child_pipe(shell, ast->left);
	else
	{
		r_pid = ft_fork(shell);
		if (r_pid == 0)
			exec_child_pipe(shell, ast->right);
		ft_close_pipe(shell);
	}
}

static void	exec_ast(t_shell *shell)
{
	t_ast	*ast;

	ast = shell->ast;
	if (ast->type == N_PIPE)
		exec_pipe(shell, ast);
	else if (ast->type == N_CMD)
		exec_cmd(shell, ast);
	else if (ast->type == N_AND)
		exec_and(shell, ast);
	else if (ast->type == N_OR)
		exec_or(shell, ast);
}
