/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:08:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/18 10:24:59 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_args(char **args, int i)
{
	int	j;

	j = 0;
	if (args && i < 0)
	{
		while ( args[j])
		{
			free(args[j++]);
		}
		free(args[j]);
	}
	else
	{
		while (args && i >= 0)
			free(args[i--]);
	}
	free(args);
}

static void ft_cmd_nf_err(char *cmd, int status)
{
	if (status == 127)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (status == 126)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else if (status == 125)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

static char *ft_get_path(t_ast *ast, char *cmd, int *status)
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
			return (*status = 1, free(to_free), free(path), ft_free_args(paths, i), NULL);
		if (access(path, F_OK) == 0)
			return (ft_free_args(paths, i), free(to_free), path);
		(free(to_free), free(path));
	}
	ft_free_args(paths, i);
	path = ft_strdup(cmd);
	if (!path)
		return (*status = 1, ft_putstr_fd("minishell: malloc: ", 2), NULL);
	if (access(cmd, F_OK) == 0 && ft_strchr(cmd, '/'))
		return (path);
	return (ft_cmd_nf_err(cmd, 127), *status = 127, NULL);
}

int is_builtin(char *cmd)
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

static int exec_builtin(t_ast *ast, char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(ast, args));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(ast, args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd(ast, args));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(ast, args));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(ast, args));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(ast, args, false));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(ast, args));
	return (0);
}

int	exec_args(t_ast *ast)
{
	char	*path;
	char	**args;
	int		status;

	args = ast->command->args;
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "\"\"") == 0 || ft_strcmp(args[0], "''") == 0)
		return (ft_cmd_nf_err(args[0], 127), 127);
	if (is_builtin(ast->command->expanded_args[0]) == 1)
		return (exec_builtin(ast, args));
	path = ft_get_path(ast, ast->command->expanded_args[0], &status);
	if (!path || !*path)
		return (status);
	ft_execve(ast, path, args);
	return (126);
}
