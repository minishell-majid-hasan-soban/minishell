/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:08:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:20:47 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_builtin(t_ast *ast, char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(ast, args));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(ast, args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd(ast, args));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(ast, args));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(ast, args));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(ast, args, false));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(ast, args));
	return (0);
}

static int	is_empty_cmd(char **args)
{
	char	*arg;
	char	*skiped;

	arg = *args;
	if (!arg)
		return (0);
	skiped = skip_quotes(arg);
	if (!skiped)
		return (1);
	if (*skiped == '\0')
		return (ft_cmd_nf_err(skiped, 127), free(skiped), 127);
	free(skiped);
	if (ft_strcmp(arg, "..") == 0)
		return (ft_cmd_nf_err(arg, 127), 127);
	return (0);
}

static int	check_empty_cmd(t_ast *ast)
{
	char	**args;
	int		status;

	args = ast->command->args;
	if (!args || !args[0])
		return (0);
	status = is_empty_cmd(args);
	if (status != 0)
		return (status);
	if (ft_strcmp(args[0], ".") == 0)
		return (ft_cmd_nf_err(args[0], 2), 2);
	return (0);
}

int	exec_args(t_ast *ast)
{
	char	*path;
	char	**args;
	int		status;

	if (!ast->command->args || !ast->command->args[0])
		return (0);
	status = check_empty_cmd(ast);
	if (status != 0)
		return (status);
	if (ast->command->expanded_args == NULL)
		return (1);
	else if (ast->command->expanded_args[0] == NULL)
		return (0);
	if (is_builtin(ast->command->expanded_args[0]) == 1)
		return (exec_builtin(ast, ast->command->globed_args));
	status = skip_null_args(ast);
	if (status != 0)
		return (status);
	args = ast->command->globed_args;
	while (*args && ft_strcmp(*args, "") == 0)
		args++;
	path = ft_get_path(ast, args[0], &status);
	if (!path || path == (void *)-1 || !*path)
		return (status);
	return (ft_execve(ast, path, args), 126);
}
