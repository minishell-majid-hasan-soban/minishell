/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:12:05 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/04 10:25:50 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_add_env(t_shell *data, char *str)
{
	char	*name;
	char	*value;
	bool	equal;

	if (ft_strchr(str, '=') == NULL)
		name = ft_strdup(str);
	else
		name = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!name)
		return (-1);
	if (ft_strchr(str, '=') == NULL)
		value = NULL;
	else
		value = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
	if (!value)
		return (free(name), -1);
	if (ft_env_addback(&data->env, name, value) < 0)
		return (free(name), free(value), -1);
	return (0);
}

int		ft_export(t_shell *data, char **args)
{
	int		i;
	char	**strs;

	i = 1;
	if (args[0] == NULL)
		return (ft_env("declare -x ", data->env, true), 0);
	while (*args)
	{
		if (ft_add_env(data, *args) < 0)
			return (-1);
		args++;
	}
	return (0);
}