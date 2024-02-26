/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:32:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 08:51:59 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

int		ft_cd(t_ast *ast, char **args);
int		ft_echo(t_ast *ast, char **args);
int		ft_export(t_ast *ast, char **args);
int		ft_exit(t_ast *ast, char **args);
int		ft_pwd(t_ast *ast, char **args);
int		ft_unset(t_ast *ast, char **args);
int		ft_env(t_ast *ast, char **args, bool export);

t_env	*ft_newenv(char *name, char *value);
int		ft_env_addback(t_env **env, char *name, char *value);
t_env	*ft_getenv(t_env *env, char *name);
int		ft_setenv(t_shell *shell, char *name, char *value, bool append);
int		ft_add_env(t_ast *ast, char *str);
char	**ft_env_to_arr(t_env *env);
char	*get_value(t_env *env, char *name);
bool	ft_valid_name(char *name);

#endif