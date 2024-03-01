/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:43:27 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/01 07:14:55 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setoldpwd(t_shell *shell)
{
	char	*name;

	name = ft_strdup("OLDPWD");
	if (name == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (ft_setenv(shell, name, NULL, false));
}

static int	ft_setpwd(t_shell *shell)
{
	char	*pwd;
	char	*tmp;

	tmp = ft_strdup("PWD");
	if (tmp == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free(tmp), ft_putstr_fd("minishell: getcwd: ", 2),
			perror(""), 1);
	return (ft_setenv(shell, tmp, pwd, false));
}

static int	ft_setshlvl(t_shell *shell)
{
	char	*shlvl;
	char	*name;
	int		lvl;
	t_env	*env;

	env = ft_getenv(shell->env, "SHLVL");
	name = ft_strdup("SHLVL");
	if (name == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
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
	if (!shlvl)
		return (free(name), ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (ft_setenv(shell, name, shlvl, false));
}

static int	ft_setunderscore(t_shell *shell)
{
	char	*underscore;
	char	*name;
	t_env	*env;

	env = ft_getenv(shell->env, "_");
	name = ft_strdup("_");
	if (name == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	underscore = ft_strdup("/usr/bin/env");
	if (underscore == NULL)
		return (free(name), ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (ft_setenv(shell, name, underscore, false));
}

int	ft_set_minimal_env(t_shell *shell)
{
	int	ret;
	int status;
	
	status = 0;
	ret = ft_setoldpwd(shell);
	status = status || ret;
	ret = ft_setpwd(shell);
	status = status || ret;
	ret = ft_setshlvl(shell);
	status = status || ret;
	ret = ft_setunderscore(shell);
	status = status || ret;
	return (status);
}
