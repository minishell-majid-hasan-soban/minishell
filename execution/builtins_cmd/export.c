/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:12:05 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/08 15:21:27 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_setenv(t_shell *shell, char *name, char *value, bool append)
{
	t_env	*tmp;
	char	*tmp2;

	tmp = ft_getenv(shell->env, name);
	if (tmp && ft_strcmp(name, "_") != 0)
	{
		if (append && tmp->value)
		{
			tmp2 = tmp->value;
			tmp->value = ft_strjoin(tmp->value, value);
			free(tmp2);
			if (tmp->value == NULL)
				return (ft_putstr_fd("minishell: malloc error\n", 2), 1);
			return (0);
		}
		free(tmp->value);
		tmp->value = ft_strdup(value);
		if (tmp->value == NULL)
			return (ft_putstr_fd("minishell: malloc error\n", 2), 1);
		return (0);
	}
	else if (tmp == NULL && ft_strcmp(name, "_") != 0)
		if (ft_env_addback(&shell->env, name, value))
			return (ft_putstr_fd("minishell: malloc error\n", 2), 1);
	return (0);
}

int	ft_add_env(t_ast *ast, char *str)
{
	char	*name;
	char	*value;
	bool	append;

	append = false;
	if (ft_strchr(str, '=') == NULL)
		name = ft_strdup(str);
	else
		name = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!name)
		return (ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
	if (!ft_valid_name(name))
		return (ast->exit_status = 1, free(name),
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2), 0);
	append = ft_strchr(str, '+') != NULL;
	name[ft_strlen(name) - append] = '\0';
	if (ft_strchr(str, '=') == NULL)
		value = NULL;
	else
		value = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
	if (!value)
		return (free(name), ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
	if (ft_setenv(ast->shell, name, value, append))
		return (free(name), free(value), 1);
	return (0);
}

int		ft_export(t_ast *ast)
{
	int		i;
	char	**args;
	char	**tmp;

	i = 1;
	args = ast->command->args;
	args++;
	tmp = args;
	if (args[0] == NULL)
		return (ft_print_env(ast->shell->env, true), 0);
	while (*args)
	{
		if (ft_add_env(ast, *args))
			return (1);
		args++;
	}
	return (0);
}
