/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:38:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 18:31:38 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_append_env(t_env *env, char *value)
{
	char	*tmp;

	tmp = env->value;
	if (tmp)
	{
		free(env->value);
		env->value = ft_strjoin(tmp, value);
		free(tmp);
		if (env->value == NULL)
			return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	else
	{
		env->value = ft_strdup(value);
		if (env->value == NULL)
			return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	return (0);
}

static int	handle_assign_env(t_env *env, char *value)
{
	free(env->value);
	env->value = value;
	return (0);
}

int	ft_setenv(t_shell *shell, char *name, char *value, bool append)
{
	t_env	*tmp;

	tmp = ft_getenv(shell->env, name);
	if (tmp)
	{
		if (append)
			return (handle_append_env(tmp, value));
		else
			return (handle_assign_env(tmp, value));
	}
	else if (tmp == NULL)
		if (ft_env_addback(&shell->env, name, value))
			return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (0);
}
