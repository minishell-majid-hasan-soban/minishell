/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:05:11 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:48:11 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmd_nf_err(char *cmd, int status)
{
	if (status == 127)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (status == 2)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
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
		ft_putstr_fd(": is a directory\n", 2);
	}
}

static char	*ft_check_path(char **paths, char *cmd, int *status)
{
	int		i;
	char	*path;
	char	*to_free;

	i = -1;
	while (paths && paths[++i])
	{
		to_free = ft_strjoin(paths[i], "/");
		path = ft_strjoin(to_free, cmd);
		if (!to_free || !path)
		{
			ft_putstr_fd(ALLOC_ERR, 2);
			return (*status = 1, free(to_free), free(path), (void *)-1);
		}
		if (access(path, X_OK) == 0)
			return (free(to_free), path);
		if (access(path, F_OK) == 0)
			return (free(to_free), path);
		(free(to_free), free(path));
	}
	return (NULL);
}

static char	*check_relative_path(char *cmd, int *status)
{
	char		*new_cmd;
	struct stat	st;

	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		return (ft_cmd_nf_err(cmd, 125), *status = 126, NULL);
	new_cmd = ft_strdup(cmd);
	if (!new_cmd)
		return (*status = 1, ft_putstr_fd(ALLOC_ERR, 2), (void *)-1);
	if (access(cmd, X_OK) == 0)
		return (new_cmd);
	free(new_cmd);
	if (access(cmd, F_OK) == 0)
		return (ft_cmd_nf_err(cmd, 126), *status = 126, NULL);
	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
		ft_putstr_fd(": No such file or directory\n", 2), *status = 127, NULL);
}

static bool	ft_split_path(char *str, int i)
{
	bool	squote;
	bool	dquote;
	int		j;

	if (str[i] != ':')
		return (false);
	squote = false;
	dquote = false;
	j = 0;
	while (str[j])
	{
		if (str[j] == '\'')
			squote = !squote;
		if (str[j] == '\"')
			dquote = !dquote;
		if (str[j] == ':' && !squote && !dquote)
			return (true);
		j++;
	}
	return (false);
}

char	*ft_get_path(t_ast *ast, char *cmd, int *status)
{
	char	**paths;
	char	*path;
	t_env	*env;

	if (!cmd)
		return (*status = 0, NULL);
	if (ft_strchr(cmd, '/'))
		return (check_relative_path(cmd, status));
	env = ft_getenv(ast->shell->env, "PATH");
	paths = NULL;
	if (env)
		paths = ft_spliter(env->value, ft_split_path);
	path = ft_check_path(paths, cmd, status);
	ft_free_args(paths);
	if (!path)
		return (ft_cmd_nf_err(cmd, 127), *status = 127, NULL);
	return (path);
}
