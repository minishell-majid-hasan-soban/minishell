/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:08:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/14 18:20:34 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_args(char **args, int i)
{
	while (i >= 0)
	{
		free(args[i]);
		i--;
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

	i = 0;
	env = ft_getenv(ast->shell->env, "PATH");
	paths = NULL;
	if (env)
		paths = ft_split(env->value, ':');
	while (paths && paths[i])
	{
		to_free = ft_strjoin(paths[i], "/");
		path = ft_strjoin(to_free, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (ft_free_args(paths, i), free(to_free), path);
		(free(to_free), free(path));
		i++;
	}
	ft_free_args(paths, i);
	if (access(cmd, F_OK | X_OK) == 0 && ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

int	exec_args(t_ast *ast, char **args)
{
	char	*path;
	char	**envp;

	path = ft_get_path(ast, args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ast->shell->exit_status = 127;
		exit(127);
	}
	ft_env_to_arr(ast->shell);
	envp = ft_env_to_arr(ast->shell);
	if (!envp)
	{
		(free(path), ft_putstr_fd("minishell: malloc error\n", 2));
		ast->shell->exit_status = 1;
		exit(1);
	}
	ft_execve(path, args, envp);
	free(path);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": ", 2);
	perror("");
	exit(126);
}
