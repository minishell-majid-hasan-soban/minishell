/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handler.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 08:44:24 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/15 08:35:47 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HANDLER_H
# define AST_HANDLER_H

# include "minishell.h"

typedef struct s_ast	t_ast;

void	ft_close(t_ast *ast, int fd);
void	ft_close_pipe(t_ast *ast, int fd[2]);
void	ft_dup2(t_ast *ast, int old_fd, int new_fd);
pid_t	ft_fork(t_ast *ast);
void	ft_pipe(t_ast *ast, int fd[2]);
void	ft_error(t_ast *ast, char *error);
int		ft_execve(t_ast *ast, char **args);
void	ft_waitpid(t_ast *ast, pid_t pid, int *status);

#endif