/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 15:01:10 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	int			i;
	t_shell		shell;
	t_ast		ast;
	char		**key_value;
	// t_env		*env;
	char		*expanded;

	(void)ac;
	(void)av;
	shell.env = NULL;
	shell.exit_status = 0;
	ast.shell = &shell;
	i = 0;
	while (envp[i])
	{
		key_value = ft_split(envp[i], '=');
		ft_env_addback(&shell.env, key_value[0], key_value[1]);
		i++;
	}
	expanded = ft_expand_arg(&ast, "zxcxzcd$HOME,777");
	printf("expanded: %s\n", expanded);
	// env = shell.env;
	// while (env)
	// {
	// 	printf("%s=%s\n", env->name, env->value);
	// 	env = env->next;
	// }
	return (0);
}
