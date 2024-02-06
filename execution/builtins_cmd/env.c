/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 02:50:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 12:45:46 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*ft_joind_kv(char *key, char *value, char *sep)
{
	char	*str;
	char	*tmp;

	tmp = ft_strjoin(key, sep);
	str = ft_strjoin(tmp, value);
	free(tmp);
	return (str);
}

char	**ft_env_to_arr(t_env *env)
{
	int		i;
	char	**envp;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **) ft_calloc(i + 1, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			envp[i]= ft_joind_kv(tmp->name, tmp->value, "=");
		i++;
		tmp = tmp->next;
	}
	return (envp);
}

void	ft_env(char *prefix, t_env *env, bool null_value)
{
	while (env)
	{
		if (null_value == true)
		{
			printf("%s%s", prefix, env->name);
			if (env->value)
				printf("=\"%s\"\n", env->value);
		}
		else if (env->value)
			printf("%s%s=\"%s\"\n", prefix, env->name, env->value);
		env = env->next;
	}
}
