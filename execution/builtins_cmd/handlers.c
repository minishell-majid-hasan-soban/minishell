/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 00:55:30 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/28 01:18:22 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	close_handle(int fd)
{
	if (close(fd) < 0)
		return (perror("close"), -1);
	return (0);
}

int	dup2_handle(int fd1, int fd2)
{
	if (close_handle(fd2) < 0)
		return (-1);
	if (dup2(fd1, fd2) < 0)
		return (close_handle(fd1), perror("dup2"), -1);
	if (close_handle(fd1) < 0)
		return (-1);
	return (0);
}

int	pipe_handle(int *pipefd)
{
	int	err;

	err = 0;
	if (close_handle(pipefd[1]) < 0)
	{
		perror("close");
		err = -1;
	}
	if (dup2_handle(pipefd[0], 0) < 0)
	{
		perror("close");
		err = -1;
	}
	return (err);
}
