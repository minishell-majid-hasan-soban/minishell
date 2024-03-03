/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:14:12 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 09:08:33 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd_error(char *arg)
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

int	ft_oldnew(t_shell *shell, char *pwd, char *oldpwd)
{
	char	*name1;
	char	*name2;

	name1 = ft_strdup("OLDPWD");
	name2 = ft_strdup("PWD");
	if (!name1 || !name2)
		return (free(pwd), free(oldpwd), free(name1),
			free(name2), ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (ft_getenv(shell->env, "OLDPWD"))
		ft_setenv(shell, name1, oldpwd, false);
	else
		(free(oldpwd), free(name1));
	if (ft_getenv(shell->env, "PWD"))
		ft_setenv(shell, name2, pwd, false);
	else
		(free(pwd), free(name2));
	return (0);
}
