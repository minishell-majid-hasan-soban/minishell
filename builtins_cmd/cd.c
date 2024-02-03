/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 01:44:11 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/28 00:24:52 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_new_env(char *name, char *value)
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

int	ft_env_addback(t_env **env, char *name, char *value)
{
	t_env	*tmp;

	tmp = *env;
	if (tmp == NULL)
	{
		*env = ft_new_env(name, value);
		if (*env == NULL)
			return (-1);
		return (0);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = ft_new_env(name, value);
	if (tmp->next == NULL)
		return (-1);
	return (0);
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

int	ft_setenv(t_env **env, char *name, char *value, int overwrite)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = ft_new_env(name, value);
		if (*env == NULL)
			return (-1);
		return (0);
	}
	tmp = ft_getenv(*env, name);
	if (tmp)
	{
		if (overwrite == 1)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			if (tmp->value == NULL)
				return (-1);
		}
		return (0);
	}
	return (ft_env_addback(env, name, value));
}

int	ft_cd(t_data *data, char **args)
{
	int		ret;
	char	*pwd;
	char	*new_pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_perror(data->name, "cd: ", ""), -1);
	if (!args || args[0] == NULL)
		new_pwd = ft_getenv(data->env, "HOME")->value;
	else if (ft_strcmp(args[0], "-") == 0)
		new_pwd = ft_getenv(data->env, "OLDPWD")->value;
	else
		new_pwd = args[0];
	ret = chdir(new_pwd);
	if (ret == -1)
		return (ft_perror(data->name, "cd: ", new_pwd), -1);
	else if (ft_strcmp(args[0], "-") == 0)
		printf("%s\n", new_pwd);
	if (ft_setenv(&data->env, "OLDPWD", pwd, 1) == -1
		|| ft_setenv(&data->env, "PWD", new_pwd, 1) == -1)
		return (ft_perror(data->name, "cd: ", ""), -1);
	return (0);
}
