/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:08:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 15:17:04 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_empty_cmd(t_ast *ast)
{
	char	**args;

	args = ast->command->args;
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "\"\"") == 0 || ft_strcmp(args[0], "''") == 0
		|| ft_strcmp(args[0], "..") == 0)
		return (ft_cmd_nf_err(ast->command->skiped_args[0], 127), 127);
	if (ft_strcmp(args[0], ".") == 0)
		return (ft_cmd_nf_err(args[0], 2), 2);
	return (0);
}

static int	skip_null_args(t_ast *ast)
{
	char	**exp;
	char	**args;

	exp = ast->command->expanded_args;
	args = ast->command->globed_args;
	while (*args && ft_strcmp(*args, "") == 0
		&& !(strchr(*exp, '\'') || strchr(*exp, '\"')))
	{
		args++;
		exp++;
	}
	if (*args && **args == '\0')
		return (ft_cmd_nf_err(ast->command->skiped_args[0], 127), 127);
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
	if (ast->command->skiped_args == NULL)
		return (1);
	if (is_builtin(ast->command->skiped_args[0]) == 1)
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
	ft_execve(ast, path, args);
	return (126);
}
