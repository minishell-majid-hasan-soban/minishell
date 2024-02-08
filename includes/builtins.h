/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:32:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/08 15:17:22 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

int		ft_cd(t_shell *data, char **args);
int		ft_echo(char **args);
int		ft_export(t_ast *ast);
int		ft_exit(t_ast *ast, char **args);

t_env	*ft_newenv(char *name, char *value);
int		ft_env_addback(t_env **env, char *name, char *value);
t_env	*ft_getenv(t_env *env, char *name);
int		ft_setenv(t_shell *shell, char *name, char *value, bool append);
void	ft_print_env(t_env *env, bool export);
int		ft_add_env(t_ast *ast, char *str);
void	ft_perror(char *name, char *str, char *suffix);
char	**ft_env_to_arr(t_env *env);
char	*get_value(t_env *env, char *name);
bool	ft_valid_name(char *name);

#endif