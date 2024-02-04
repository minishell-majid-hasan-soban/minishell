/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   her_doc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:38:20 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/04 10:25:50 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	her_doc_process(t_shell *data, char **args, int fd[2])
{
	char	*line;

	if (close_handle(fd[0]) < 0)
		return (ft_perror(data->name, "close: ", ""), exit(1), -1);
	while (1)
	{
		line = get_next_line(data->fd_in);
		if (ft_strcmp(line, args[1]) == 0 || !line || line == (char *)-1)
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	if (line == (char *)-1)
		return (ft_perror(data->name, "malloc: ", ""), exit(1), -1);
	if (close_handle(fd[1]) < 0)
		return (free(line), ft_perror(data->name, "close: ", ""), exit(1), -1);
	free(line);
	exit(0);
}

int	her_doc(char **args, t_shell *data)
{
	int		fd[2];
	char	*line;
	pid_t	pid;

	if (!args || !*args)
		return (ft_perror(data->name, "", "parse error"), -1);
	if (pipe(fd) < 0)
		return (ft_perror(data->name, "pipe: ", ""), -1);
	pid = fork();
	if (pid < 0)
		return (ft_perror(data->name, "fork: ", ""), -1);
	if (pid == 0)
		return (her_doc_process(data, args, fd));
	if (close_handle(fd[1]) < 0)
		return (ft_perror(data->name, "close: ", ""), -1);
	if (wait(&data->status) < 0)
		return (ft_perror(data->name, "wait: ", ""), -1);
	if (dup2(fd[0], 0) < 0)
		return (ft_perror(data->name, "dup2: ", ""), -1);
	return (0);
}
