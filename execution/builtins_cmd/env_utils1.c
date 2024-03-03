/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:38:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 09:03:22 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_append_env(t_env *env, char *value)
{
	char	*tmp;

	tmp = env->value;
	if (tmp)
	{
		env->value = ft_strjoin(tmp, value);
		(free(tmp), free(value));
		if (env->value == NULL)
			return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	else
		env->value = value;
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

	if (name == NULL)
		return (0);
	tmp = ft_getenv(shell->env, name);
	if (tmp)
	{
		free(name);
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

int	ft_get_shlvl(char *str)
{
	long long	r;
	int			s;

	r = 0;
	s = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		s = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		r = r * 10 + *str - '0';
		if (r > 999 && s == 1)
			return (0);
		str++;
	}
	if (*str)
		return (0);
	if (s == -1)
		return (-1);
	return (r);
}
