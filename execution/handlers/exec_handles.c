/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:28:33 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/04 11:42:05 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(t_shell *shell, int fd)
{
	if (close(fd) == -1)
	{
		shell->error = T_FUNC;
		ft_error(shell, strerror(errno));
	}
}

void	ft_close_pipe(t_shell *shell)
{
	ft_close(shell, shell->p_fd[0]);
	ft_close(shell, shell->p_fd[1]);
}

void	ft_dup2(t_shell *shell, int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1 && shell->error == T_NONE)
	{
		shell->error = T_FUNC;
		ft_error(shell, strerror(errno));
	}
}

pid_t	ft_fork(t_shell *shell)
{
	pid_t	pid;

	if (shell->error != T_NONE)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		shell->error = T_FUNC;
		ft_error(shell, strerror(errno));
	}
	return (pid);
}

void	ft_pipe(t_shell *shell)
{
	if (pipe(shell->p_fd) == -1 && shell->error == T_NONE)
	{
		shell->error = T_FUNC;
		ft_error(shell, strerror(errno));
	}
}
