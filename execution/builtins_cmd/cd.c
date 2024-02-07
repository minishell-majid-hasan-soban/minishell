/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:44:11 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/07 13:36:46 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setenv(t_shell *shell, char *name, char *value, bool overwrite)
{
	t_env	*tmp;

	if (!ft_valid_name(name))
		return (ft_putstr_fd("minishell: cd: `", 2), ft_putstr_fd(name, 2),
			ft_putstr_fd("': not a valid identifier\n", 2), 0);
	tmp = ft_getenv(shell->env, name);
	if (tmp && overwrite && ft_strcmp(name, "_") != 0)
	{
		free(tmp->value);
		tmp->value = ft_strdup(value);
		if (tmp->value == NULL)
			return (ft_putstr_fd("minishell: cd: malloc error\n", 2), 1);
		return (0);
	}
	else if (tmp == NULL && ft_strcmp(name, "_") != 0)
	{
		if (ft_env_addback(&shell->env, name, value))
			return (ft_putstr_fd("minishell: cd: malloc error\n", 2), 1);
	}
	return (0);
}

int	ft_cd(t_shell *shell, char **args)
{
	int		ret;
	char	*pwd;
	char	*new_pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_putstr_fd("minishell: cd: malloc error\n", 2), 1);
	if (args[1] == NULL)
	{
		if (ft_getenv(shell->env, "HOME") == NULL)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
		new_pwd = ft_getenv(shell->env, "HOME")->value;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (ft_getenv(shell->env, "OLDPWD") == NULL)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		new_pwd = ft_getenv(shell->env, "OLDPWD")->value;
	}
	else
		new_pwd = args[1];
	ret = chdir(new_pwd);
	if (ret == -1)
		return (ft_putstr_fd("minishell: cd: ", 2), ft_putstr_fd(new_pwd, 2),
			ft_putstr_fd(": No such file or directory\n", 2), 1);
	else if (ft_strcmp(args[0], "-") == 0)
		printf("%s\n", new_pwd);
	if (ft_setenv(shell, "OLDPWD", pwd, 1)
		|| ft_setenv(shell, "PWD", new_pwd, 1))
		return (ft_perror(shell->name, "cd: ", ""), -1);
	return (0);
}
