/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:08:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/15 11:06:08 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_args(char **args, int i)
{
	int	j;

	j = 0;
	if (i < 0)
	{
		while (args[j])
			free(args[j++]);
		free(args[j]);
	}
	else
	{
		while (i >= 0)
			free(args[i--]);
	}
	free(args);
}

static char *ft_get_path(t_ast *ast, char *cmd)
{
	char	**paths;
	char	*path;
	char	*to_free;
	int		i;
	t_env	*env;

	i = -1;
	env = ft_getenv(ast->shell->env, "PATH");
	paths = NULL;
	if (env)
		paths = ft_split(env->value, ':');
	while (paths && paths[++i])
	{
		to_free = ft_strjoin(paths[i], "/");
		path = ft_strjoin(to_free, cmd);
		if (!to_free || !path)
			return (free(to_free), free(path), ft_free_args(paths, i), NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (ft_free_args(paths, i), free(to_free), path);
		(free(to_free), free(path));
	}
	ft_free_args(paths, i);
	if (access(cmd, F_OK | X_OK) == 0 && ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static void ft_cmd_nf_err(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int is_builtin(t_ast *ast, char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static exec_builtin(t_ast *ast, char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(ast->shell, args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd(ast->shell, args));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(ast->shell, args));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(ast->shell, args));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(ast->shell, args));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(ast->shell, args));
	return (0);
}

int	exec_args(t_ast *ast, char **args)
{
	char	*path;
	char	**envp;

	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "\"\"") || ft_strcmp(args[0], "''"))
		return (ft_cmd_nf_err(args[0]), 127);
	path = ft_get_path(ast, args[0]);
	if (!path || !*path)
		return (0);
	if (is_builtin(ast, path) == 1)
		return (ft_exec_builtin(ast, args));
	ft_execve(path, args);
	free(path);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": ", 2);
	perror("");
	return (126);
}
