/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/14 18:34:39 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include <libc.h>

// char	**ft_duparr(char **arr)
// {
// 	int		i;
// 	char	**new;

// 	i = 0;
// 	while (arr[i])
// 		i++;
// 	new = (char **)malloc(sizeof(char *) * (i + 1));
// 	if (new == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (arr[i])
// 	{
// 		new[i] = ft_strdup(arr[i]);
// 		i++;
// 	}
// 	new[i] = NULL;
// 	return (new);
// }

// int	ft_fill_env(t_env **env, char *str)
// {
// 	char	*name;
// 	char	*value;

// 	if (ft_strchr(str, '=') == NULL)
// 		name = ft_strdup(str);
// 	else
// 		name = ft_substr(str, 0, ft_strchr(str, '=') - str);
// 	if (!name)
// 		return (ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
// 	if (ft_strchr(str, '=') == NULL)
// 		value = NULL;
// 	else
// 		value = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
// 	if (!value)
// 		return (free(name), ft_putstr_fd("minishell: export: malloc error\n", 2), 1);
// 	if (!*env)
// 		*env = ft_newenv(name, value);
// 	else
// 		ft_env_addback(env, name, value);
// 	return (0);
// }

# define S 300000

int main()
{
	// int		ret;
	// int		i;

	// chdir("a/b/c/d");
	// sleep(3);
	// char *p = getcwd(NULL, 0);
	// printf("getcwd : %s\n", p);
	// printf("PWD %s\n", getenv("PWD"));
	// printf("ret: %d\n", ret);
	// printf("getcwd PWD : %s\n", getcwd(NULL, 0));
	// sleep(1);
	// i = 0;
	// while (i < 5)
	// {
	// 	sleep(1);
	// 	ret = chdir("..");
	// 	printf("ret: %d\n", ret);
	// 	printf("getcwd PWD : %s\n\n", getcwd(NULL, 0));
	// 	i++;
	// }
	// int p = execve("", (char *[]){"", NULL}, NULL);
	// perror("execve");
	// printf("p: %d\n", p);
	return (0);
}
