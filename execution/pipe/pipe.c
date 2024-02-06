/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 04:32:32 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 11:02:01 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_process(t_shell *data, char **args, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ft_perror(data->name, "fork: ", ""), -1);
	if (pid == 0)
	{
		if (close_handle(fd[0]) < 0)
			return (ft_perror(data->name, "close: ", ""), exit(1), -1);
		if (dup2(fd[1], 1) < 0)
			return (ft_perror(data->name, "dup2: ", ""), exit(1), -1);
		if (execve(args[0], args, NULL) < 0)
			return (ft_perror(data->name, "execve: ", ""), exit(1), -1);
	}
	if (close_handle(fd[1]) < 0)
		return (ft_perror(data->name, "close: ", ""), -1);
	if (dup2(fd[0], 0) < 0)
		return (ft_perror(data->name, "dup2: ", ""), -1);
	return (0);
}

