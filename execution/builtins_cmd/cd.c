/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:44:11 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 10:14:42 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setoldnew_pwd(t_shell *shell, char *pwd, char *oldpwd)
{
	if (!oldpwd || !pwd)
		return (free(pwd), free(oldpwd), ft_putstr_fd("minishell: cd: ", 2),
			perror(""), 1);
	if (ft_getenv(shell->env, "OLDPWD"))
		ft_setenv(shell, "OLDPWD", oldpwd, false);
	else
		free(oldpwd);
	if (ft_getenv(shell->env, "PWD"))
		ft_setenv(shell, "PWD", pwd, false);
	else
		free(pwd);
	return (0);
}

static int	go_home(t_shell *shell)
{
	t_env	*tmp;
	char	*oldpwd;

	tmp = ft_getenv(shell->env, "HOME");
	if (tmp == NULL)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(tmp->value) == -1)
		return (free(oldpwd), ft_cd_error(tmp->value), 1);
	return (ft_setoldnew_pwd(shell, getcwd(NULL, 0), oldpwd));
}

static int	parent_dir(t_shell *shell)
{
	char		*pwd;
	char		*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (oldpwd == NULL)
		return (chdir("/"), ft_cd_error(".."), 1);
	if (chdir("..") != -1)
		return (ft_setoldnew_pwd(shell, getcwd(NULL, 0), oldpwd));
	pwd = ft_strdup("/");
	ft_setoldnew_pwd(shell, pwd, oldpwd);
	if (chdir("/") == -1)
		return (ft_cd_error("/"), 1);
	return (0);
}

static int	ft_cd_oldpwd(t_shell *shell)
{
	t_env	*tmp;
	char	*oldpwd;
	char	*pwd;

	tmp = ft_getenv(shell->env, "OLDPWD");
	if (tmp == NULL || tmp->value == NULL)
		return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(tmp->value) == -1)
		return (free(oldpwd), ft_cd_error(tmp->value), 1);
	pwd = getcwd(NULL, 0);
	return (ft_putendl_fd(pwd, 1), ft_setoldnew_pwd(shell, pwd, oldpwd));
}

int	ft_cd(t_ast *ast, char **args)
{
	char	*oldpwd;

	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		return (go_home(ast->shell));
	else if (ft_strcmp(args[1], "..") == 0)
		return (parent_dir(ast->shell));
	else if (ft_strcmp(args[1], "-") == 0)
		return (ft_cd_oldpwd(ast->shell));
	oldpwd = getcwd(NULL, 0);
	if (chdir(args[1]) == -1)
		return (free(oldpwd), ft_cd_error(args[1]), 1);
	return (ft_setoldnew_pwd(ast->shell, getcwd(NULL, 0), oldpwd));
}
