/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/07 14:52:12 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libc.h>

char	**ft_duparr(char **arr)
{
	int		i;
	char	**new;

	i = 0;
	while (arr[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		new[i] = ft_strdup(arr[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

int	ft_fill_env(t_env **env, char *str)
{
	char	*name;
	char	*value;

	if (ft_strchr(str, '=') == NULL)
		name = ft_strdup(str);
	else
		name = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!name)
		return (ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
	if (ft_strchr(str, '=') == NULL)
		value = NULL;
	else
		value = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
	if (!value)
		return (free(name), ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
	if (!*env)
		*env = ft_newenv(name, value);
	else
		ft_env_addback(env, name, value);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	int			i;
	t_shell		shell;
	t_ast		ast;
	t_command	command;
	char		**args;
	// t_env		*env;
	// char		*expanded;

	(void)ac;
	shell.env = NULL;
	shell.exit_status = 0;
	ast.shell = &shell;
	args = ft_duparr(av);
	command.args = args;
	ast.command = &command;
	shell.ast = &ast;
	
	return (0);
}
