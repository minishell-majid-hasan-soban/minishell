/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:12:05 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 10:12:01 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_prit_inval_name(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	ft_handle_plus(t_ast *ast, char *name, char *value)
{
	name[ft_strlen(name) - 1] = '\0';
	if (ft_setenv(ast->shell, name, value, true))
		return (free(name), free(value), 1);
	return (0);
}

int	ft_add_env(t_ast *ast, char *str)
{
	char	*name;
	char	*value;

	if (ft_strchr(str, '=') == NULL)
		name = ft_strdup(str);
	else
		name = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!name)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	if (!ft_valid_name(name))
		return (free(name), ft_prit_inval_name(str), 1);
	value = NULL;
	if (ft_strchr(str, '=') != NULL)
	{
		value = ft_substr(str, ft_strchr(str, '=') - str + 1,
				ft_strrchr(str, 0) - ft_strchr(str, '='));
		if (!value)
			return (free(name), ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	if (ft_strchr(name, '+') != NULL)
		return (ft_handle_plus(ast, name, value));
	else if (ft_strchr(name, '+') == NULL
		&& ft_setenv(ast->shell, name, value, false))
		return (free(name), free(value), 1);
	return (0);
}

int	ft_export(t_ast *ast, char **args)
{
	int		status;

	status = 0;
	args++;
	if (args[0] == NULL)
		return (ft_env(ast, NULL, true), 0);
	while (*args)
	{
		if (ft_add_env(ast, *args))
			status = 1;
		args++;
	}
	return (status);
}
