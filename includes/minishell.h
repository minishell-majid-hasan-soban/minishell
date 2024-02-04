/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/28 03:45:31 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>

typedef struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct		s_data
{
	t_env			*env;
	char			*name;
	int				fd_in;
	int				fd_out;
	int				status;
}					t_data;

// // string_utils
int		ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s1);
int		ft_putstr_fd(char *s, int fd);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_substr(char const *s, unsigned int start, size_t len);

// // builtins_cmd
int		ft_pwd(t_data *data);
int		ft_echo(char **args);
int		ft_cd(t_data *data, char **args);
void	ft_perror(char *name, char *str, char *suffix);
int		nbr_args(char **args);
void	ft_env(char *prefix, t_env *env, bool null_value);
int		ft_unset(char **args, t_data *data);
void	ft_exit(t_data *data);
int		ft_export(t_data *data, char **args);
int		ft_env_addback(t_env **env, char *name, char *value);

// // redirections
int		red_in(char **args, t_data *data);

// // hanlders
int		dup2_handle(int fd1, int fd2);
int		close_handle(int fd);
int		pipe_handle(int *pipefd);

#endif