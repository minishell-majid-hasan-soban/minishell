/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:31:15 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 17:37:11 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_read_here_doc(int fd_w, char *limiter)
{
	char			*line;
	
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (0);
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		(ft_putstr_fd(line, fd_w), ft_putstr_fd("\n", fd_w), free(line));
	}
	return (close(fd_w));
}

int	init_here_doc(char *limiter)
{
	int				fd_w;
	int				fd_r;

	fd_w = open("/tmp/.minishell_heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_w == -1)
		return (ft_putstr_fd("minishell: ", 2),
			ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), -1);
	fd_r = open("/tmp/.minishell_heredoc", O_RDONLY);
	if (fd_r == -1)
		return (ft_close(NULL, fd_w), ft_putstr_fd("minishell: ", 2),
			ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), -1);
	unlink("/tmp/.minishell_heredoc");
	if (ft_read_here_doc(fd_w, limiter))
		return (ft_putstr_fd("minishell: ", 2),
			ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), -1);
	return (fd_r);
}