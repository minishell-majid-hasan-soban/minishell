/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:31:15 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 10:34:26 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"



int	ft_read_here_doc(t_ast *ast, int fd_w, char *limiter, bool expand)
{
	char			*line;
	char			*tmp;
	
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (0);
		tmp = skip_quotes(limiter);
		if (ft_strcmp(line, tmp) == 0)
		{
			free(line);
			break ;
		}
		free(tmp);
		if (expand == true)
		{
			tmp = ft_expand_arg(ast, line);
			if (!tmp)
				return (ft_putstr_fd(ALLOC_ERR, 2), 1);
			ft_putstr_fd(tmp, fd_w);
			free(tmp);
		}
		else
			ft_putstr_fd(line, fd_w);
		(ft_putstr_fd("\n", fd_w), free(line));
	}
	return (close(fd_w));
}

int	ft_fork_heredoc(t_ast *ast, int fd_w, char *limiter, bool expand)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("minishell: ", 2),
			ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), -1);
	if (pid == 0)
	{
		signal(SIGINT, sig_heredoc_handler);
		signal(SIGQUIT, SIG_IGN);
		status = ft_read_here_doc(ast, fd_w, limiter, expand);
		if (status != 0)
		{
			if (status == -1)
				(ft_putstr_fd("minishell: close: ", 2), perror(""));
			exit(1);
		}
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	init_here_doc(t_ast *ast, char *limiter, int *status)
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
	*status = ft_fork_heredoc(ast, fd_w, limiter,(ft_strchr(limiter, '\'') == NULL
		&& ft_strchr(limiter, '\"') == NULL));
	if (*status != 0)
		return (ft_close(ast, fd_w), ft_close(ast, fd_r), -1);
	return (ft_close(ast, fd_w), fd_r);
}
