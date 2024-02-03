/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 07:37:25 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libc.h>

void	f()
{
	system("leaks minishell");
}

void t(char *str)
{
	printf("%s\n", str);
}

int main(int ac, char **av, char **env) {

	char *str;
	t_data *data = (t_data *)malloc(sizeof(t_data));
	data->env = (t_env *)malloc(sizeof(t_env));
	data->name = av[0];
	data->env->name = ft_strdup("HOME");
	data->env->value = ft_strdup("/Users/hsobane");
	data->env->next = NULL;
	
	while (1)
	{
		add_history(str);
		str = readline("minishell$ ");
		if (ft_strcmp(str, "pwd") == 0)
			ft_pwd(data);
		else if (ft_strcmp(str, "echo") == 0)
			ft_echo((char *[]){NULL});
		else if (ft_strcmp(str, "cd") == 0)
			ft_cd(data, (char *[]){NULL});
		else if (ft_strcmp(str, "env") == 0)
			ft_env("", data->env, false);
		else if (ft_strcmp(str, "unset") == 0)
			ft_unset((char *[]){"HOME"}, data);
		else if (ft_strcmp(str, "exit") == 0)
			ft_exit(data);
		else
			printf("minishell: command not found: %s\n", str);
		free(str);
	}
}
