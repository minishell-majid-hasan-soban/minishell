/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:14:12 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 10:14:36 by hsobane          ###   ########.fr       */
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
