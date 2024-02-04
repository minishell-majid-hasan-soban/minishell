/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 02:50:37 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 04:31:38 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
