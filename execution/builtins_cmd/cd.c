/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:44:11 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/23 10:53:36 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setpwd(t_shell *shell, char *pwd)
{
	char	*tmp;

	tmp = ft_getenv(shell->env, "PWD")->value;
	ft_setenv(shell, "OLDPWD", tmp, 0);
	ft_setenv(shell, "PWD", pwd, 0);
	return (0);
}

static void	ft_cd_error(char *arg)
{
	if (ft_strcmp(arg, "..") == 0)
	{
		ft_putstr_fd("cd: error retrieving current directory:", 2);
		ft_putstr_fd(" getcwd: cannot access parent directories:", 2);
		ft_putstr_fd(" No such file or directory\n", 2);
	}
	else if (ft_strcmp(arg, "malloc") == 0)
		ft_putstr_fd("minishell: cd: malloc error\n", 2);
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
}

static int	handle_env(t_shell *shell, char *arg)
{
	t_env	*tmp;

	tmp = NULL;
	if (arg == 0 || ft_strcmp(arg, "~") == 0)
	{
		tmp = ft_getenv(shell->env, "HOME");
		if (tmp == NULL)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else
	{
		tmp = ft_getenv(shell->env, "OLDPWD");
		if (tmp == NULL)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	}
	if (chdir(tmp->value) == -1)
		return (ft_cd_error(tmp->value), 1);
	if (ft_getenv(shell->env, "PWD"))
		ft_setenv(shell, "OLDPWD", ft_getenv(shell->env, "PWD")->value, 0);
	else
		ft_setenv(shell, "OLDPWD", "/", 0);
	ft_setenv(shell, "PWD", tmp->value, 0);
	return (0);
}

static int	handle_perent_dir(t_shell *shell)
{
	char	*pwd;
	char	*new_pwd;
	char	*tmp;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_cd_error(".."), 1);
	new_pwd = ft_strrchr(pwd, '/');
	if (new_pwd == NULL)
		return (free(pwd), ft_cd_error(".."), 1);
	new_pwd = ft_substr(pwd, 0, new_pwd - pwd);
	if (new_pwd == NULL)
		return (free(pwd), ft_cd_error("malloc"), 1);
	if (chdir(new_pwd) == -1 && ft_strcmp(new_pwd, "/") != 0)
	{
		tmp = ft_strjoin(new_pwd, "/..");
		if (tmp == NULL)
			return (free(new_pwd), ft_cd_error("malloc"), 1);
		(ft_setpwd(shell, tmp), free(new_pwd), free(tmp));
		return (ft_cd_error(new_pwd), 1);
	}
	return (ft_setpwd(shell, new_pwd), free(new_pwd), 0);
}

int	ft_cd(t_ast *ast, char **args)
{
	char	*pwd;

	if (args[1] == NULL || ft_strcmp(args[1], "-") == 0
		|| ft_strcmp(args[1], "~") == 0)
		return (handle_env(ast->shell, args[1]));
	else if (ft_strcmp(args[1], "..") == 0)
		return (handle_perent_dir(ast->shell));
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_cd_error(".."), 1);
	if (chdir(args[1]) == -1)
		return (free(pwd), ft_cd_error(args[1]), 1);
	ft_setpwd(ast->shell, pwd);
	return (free(pwd), 0);
}
