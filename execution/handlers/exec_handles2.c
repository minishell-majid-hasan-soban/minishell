/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handles2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 08:24:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/15 08:35:18 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_waitpid(t_ast *ast, pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		ft_putstr_fd("minishell: waitpid: ", 2);
		perror("");
		*status = 1;
	}
	else
	{
		if (WIFEXITED(*status))
			*status = WEXITSTATUS(*status);
	}
}
