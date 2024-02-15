/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:59:30 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/15 16:27:32 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_newenv(char *name, char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (env == NULL)
		return (NULL);
	env->name = name;
	env->value = value;
	env->next = NULL;
	return (env);
}

t_env	*ft_getenv(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
int	ft_env_addback(t_env **env, char *name, char *value)
{
	t_env	*tmp;
	t_env	*new;

	new = ft_newenv(name, value);
	if (new == NULL)
		return (1);
	tmp = *env;
	if (tmp == NULL)
		return (*env = new, 0);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (0);
}

bool	ft_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (false);
	i++;
	while (name[i])
	{
		if (name[i] == '+' && name[i + 1] == 0)
			return (true);
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
