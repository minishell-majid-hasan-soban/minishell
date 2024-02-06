/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 13:57:47 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	int			i;
	t_shell		shell;
	// t_ast		ast;
	char		**key_value;

	(void)ac;
	(void)av;
	shell.env = NULL;
	i = 0;
	while (envp[i])
	{
		key_value = ft_split(envp[i], '=');
		// printf("key: %s, value: %s\n", key_value[0], key_value[1]);
		ft_env_addback(&shell.env, key_value[0], key_value[1]);
		i++;
	}
	while (shell.env)
	{
		printf("key: %s, value: %s\n", shell.env->name, shell.env->value);
		shell.env = shell.env->next;
	}
	return (0);
}
