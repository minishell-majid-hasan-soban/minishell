/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:32:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 12:46:02 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

t_env	*ft_new_env(char *name, char *value);
int		ft_env_addback(t_env **env, char *name, char *value);
t_env	*ft_getenv(t_env *env, char *name);
int		ft_setenv(t_env **env, char *name, char *value, int overwrite);
int		ft_cd(t_shell *data, char **args);
int		ft_echo(char **args);
void	ft_env(char *prefix, t_env *env, bool null_value);
void	ft_exit(t_shell *data);
int		ft_add_env(t_shell *data, char *str);
int		ft_export(t_shell *data, char **args);
void	ft_perror(char *name, char *str, char *suffix);
char	**ft_env_to_arr(t_env *env);
char	*get_value(t_env *env, char *name);

#endif