/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 04:32:32 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/28 04:41:24 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pipe_process(t_data *data, char **args, int fd[2])
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

int	ft_pipe(char **args, t_data *data, bool last)
{
	int		fd[2];
	pid_t	pid;

	if (!args || !*args)
		return (ft_perror(data->name, "", "parse error"), -1);
	if (pipe(fd) < 0)
		return (ft_perror(data->name, "pipe: ", ""), -1);
	pid = fork();
	if (pid < 0)
		return (ft_perror(data->name, "fork: ", ""), -1);
	if (pid == 0)
		return (pipe_process(data, args, fd));
	if (close_handle(fd[0]) < 0)
		return (ft_perror(data->name, "close: ", ""), -1);
	if (wait(&data->status) < 0)
		return (ft_perror(data->name, "wait: ", ""), -1);
	if (dup2(fd[1], 1) < 0)
		return (ft_perror(data->name, "dup2: ", ""), -1);
	return (0);
}
