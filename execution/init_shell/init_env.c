/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:43:27 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 10:43:51 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setpwd(t_shell *shell)
{
	char	*pwd;
	char	*tmp;
	t_env	*env;

	env = ft_getenv(shell->env, "PWD");
	if (env)
		tmp = env->name;
	else
		tmp = ft_strdup("PWD");
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_putstr_fd("minishell: getcwd: ", 2), perror(""), 1);
	if (!tmp)
		return (free(pwd), ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (ft_setenv(shell, tmp, pwd, false))
		return (1);
	return (0);
}

static int	ft_setshlvl(t_shell *shell)
{
	char	*shlvl;
	char	*tmp;
	int		lvl;
	t_env	*env;

	env = ft_getenv(shell->env, "SHLVL");
	if (env)
		tmp = env->name;
	else
		tmp = ft_strdup("SHLVL");
	if (env && env->value)
	{
		lvl = ft_atoi(env->value);
		if (lvl < 0)
			lvl = 0;
		lvl++;
	}
	else
		lvl = 1;
	shlvl = ft_itoa(lvl);
	if (!shlvl || !tmp)
		return (free(tmp), free(shlvl), ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (ft_setenv(shell, tmp, shlvl, false))
		return (1);
	return (0);
}

static int	ft_setunderscore(t_shell *shell)
{
	char	*underscore;
	char	*tmp;
	t_env	*env;

	env = ft_getenv(shell->env, "_");
	if (env)
		tmp = env->name;
	else
		tmp = ft_strdup("_");
	underscore = ft_strdup("/usr/bin/env");
	if (underscore == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (tmp == NULL)
		return (free(underscore), ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (ft_setenv(shell, tmp, underscore, false))
		return (1);
	return (0);
}

int	ft_set_minimal_env(t_shell *shell)
{
	char	*oldpwd;

	oldpwd = ft_strdup("OLDPWD");
	if (oldpwd == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	ft_setenv(shell, oldpwd, NULL, false);
	ft_setpwd(shell);
	ft_setshlvl(shell);
	ft_setunderscore(shell);
	return (0);
}
