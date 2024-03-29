/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 17:04:16 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/02 17:59:35 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_input(t_shell *shell)
{
	if (isatty(STDIN_FILENO) == 0)
		shell->tty = false;
	else
		shell->tty = true;
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static char	*handle_amb_redir(char *file, char **file_globed)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	ft_free_args(file_globed);
	return (NULL);
}

static char	*get_file(char **file_globed)
{
	char	*file;

	if (!*file_globed)
		return (ft_free_args(file_globed), NULL);
	file = ft_strdup(*file_globed);
	ft_free_args(file_globed);
	if (!file)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	return (file);
}

char	*ft_file_param(t_ast *ast, char *file)
{
	char	**file_globed;
	char	**exp;
	int		ret;

	ret = 0;
	exp = ft_expand_arg(ast, file);
	if (ast->error != T_NONE)
		return (ft_free_args(exp), NULL);
	if (!exp && ast->command->args)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	else if (ft_argslen(exp) == 0 || ft_argslen(exp) > 1)
		return (handle_amb_redir(file, exp));
	file_globed = ft_calloc(2, sizeof(char *));
	if (!file_globed)
		return (ft_free_args(exp), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	if (ft_glob_arg(ast, exp[0], &file_globed))
		return (ft_free_args(exp), ft_free_args(file_globed), NULL);
	if (!*exp[0] || !is_quoted(file, ' ', true) || ft_argslen(file_globed) > 1)
		return (handle_amb_redir(exp[0], file_globed), ft_free_args(exp), NULL);
	return (ft_free_args(exp), get_file(file_globed));
}
