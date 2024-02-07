/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 02:50:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/07 10:55:34 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
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

void	ft_print_env(t_env *env, bool export)
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		if (export)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->name, tmp->value);
			else
				printf("declare -x %s\n", tmp->name);
		}
		else if (tmp->value)
			printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
}
