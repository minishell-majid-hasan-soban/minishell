/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:31:15 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 20:39:31 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	ft_eof_not(char *line, char *limiter, int fd)
{
	if (line == NULL)
		return (1);
	if (ft_strcmp(line, limiter) == 0)
		return (1);
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
	return (0);
}

static int	ft_eof_exp(t_ast *ast, char *line, char *limiter, int fd)
{
	char	*skipped;

	if (line == NULL)
		return (1);
	if (ft_strcmp(line, limiter) == 0)
		return (1);
	skipped = expand_heredoc(ast, line);
	if (!skipped)
		return (ft_putstr_fd(ALLOC_ERR, 2), -11);
	ft_putstr_fd(skipped, fd);
	ft_putstr_fd("\n", fd);
	free(skipped);
	free(line);
	return (0);
}

int	ft_read_here_doc(t_ast *ast, int fd_w, char *limiter, bool expand)
{
	char			*line;
	char			*skip_limiter;
	int				ret;

	skip_limiter = skip_quotes(limiter);
	if (skip_limiter == NULL)
		return (close(fd_w), ft_putstr_fd(ALLOC_ERR, 2), -11);
	ret = 0;
	while (1)
	{
		line = readline("> ");
		if (expand == true)
			ret = ft_eof_exp(ast, line, skip_limiter, fd_w);
		else
			ret = ft_eof_not(line, skip_limiter, fd_w);
		if (ret != 0)
			break ;
	}
	if (close(fd_w) == -1)
		return (ft_putstr_fd("minishell: close: ", 2), perror(""), 1);
	return (ret == -11);
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
			exit(42);
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
	*status = ft_fork_heredoc(ast, fd_w, limiter,
			(ft_strchr(limiter, '\'') == NULL
				&& ft_strchr(limiter, '\"') == NULL));
	if (*status != 0)
		return (ft_close(ast, fd_w), ft_close(ast, fd_r), -1);
	return (ft_close(ast, fd_w), fd_r);
}
