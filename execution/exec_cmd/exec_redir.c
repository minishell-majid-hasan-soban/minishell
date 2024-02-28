/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:33:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 11:09:08 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_redir_input(t_ast *ast, t_redirection *redir)
{
	int		fd;
	char	*file;

	file = ft_file_param(ast, redir->file);
	if (!file || !*file)
		return (free(file), 1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		free(file);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	free(file);
	ft_dup2(ast, fd, 0);
	ft_close(ast, fd);
	return (ast->error != T_NONE);
}

static int	exec_redir_output(t_ast *ast, t_redirection *redir)
{
	int		fd;
	char	*file;

	file = ft_file_param(ast, redir->file);
	if (!file || !*file)
		return (1);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		free(file);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	free(file);
	ft_dup2(ast, fd, 1);
	ft_close(ast, fd);
	return (ast->error != T_NONE);
}

static int	exec_redir_append(t_ast *ast, t_redirection *redir)
{
	int		fd;
	char	*file;

	file = ft_file_param(ast, redir->file);
	if (!file || !*file)
		return (free(file), 1);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		free(file);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	free(file);
	ft_dup2(ast, fd, 1);
	ft_close(ast, fd);
	return (ast->error != T_NONE);
}

static int	exec_redir_heredoc(t_ast *ast, t_redirection *redir)
{
	ft_dup2(ast, redir->heredoc_fd, 0);
	ft_close(ast, redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (ast->error != T_NONE);
}

int	exec_redir(t_ast *ast, t_redirection *redir)
{
	int				ret;

	ret = 0;
	while (redir)
	{
		if (redir->type != R_HEREDOC && (!redir->file || !*redir->file))
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd("No such file or directory\n", 2), 1);
		if (redir->type == R_INPUT)
			ret = exec_redir_input(ast, redir);
		else if (redir->type == R_OUTPUT)
			ret = exec_redir_output(ast, redir);
		else if (redir->type == R_APPEND)
			ret = exec_redir_append(ast, redir);
		else if (redir->type == R_HEREDOC)
			ret = exec_redir_heredoc(ast, redir);
		if (ret)
			return (1);
		redir = redir->next;
	}
	return (0);
}
