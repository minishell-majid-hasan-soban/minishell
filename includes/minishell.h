/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/06 21:34:20 by amajid           ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include "libft.h"
# include "enum.h"

extern bool	g_sigint;

/*
** t_token: struct for tokens:
** value: the value of the token if it's a word, like "echo" or "file" or "argument"
** type: the type of the token
** error: the error of the token, if TOKEN_NEWLINE is followed by nothing, it's a syntax error
** next: the next token, list easy a sat
** prev: the previous token
*/
typedef struct	s_token
{
	char				*value;
	int					type;
	t_error				error;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_token_arr
{
	t_token	*arr;
	long	size;
	long	count;
}	t_token_arr;


/*
** t_redirection: struct for redirections:
** type: the type of the redirection, like R_INPUT or R_OUTPUT [t_redirection_type enum above]
** file: the file of the redirection, like "file" in "echo hello > file"
** heredoc_fd: used if the redirection is a heredoc, it hodls the read end of the pipe
** next: the next redirection
** prev: the previous redirection
*/
typedef struct	s_redirection
{
	t_redirection_type		type;
	char					*file;
	int						heredoc_fd;
	bool					expanded;
	char					*expanded_file;
	struct	s_redirection	*next;
	struct	s_redirection	*prev;
	t_error					error;
}							t_redirection;

/*
** t_command: struct for commands:
** name: the name of the command, like "echo" or "ls"
** args: the arguments of the command, like "echo hello" -> "hello" is the argument
** redirections: the redirections of the command, like "echo hello > file" -> "file" is the redirection of type R_OUTPUT
** t_redirection: is above this struct
*/
typedef struct	s_command
{
	char			*name;
	char			**args;
	long			arg_count;
	long			arg_size;
	char			*expanded_name;
	char			**expanded_args;
	t_redirection	*redirections;
	t_builtin		cmd_type;
	t_error			error;
}					t_command;

/*
** t_node: struct for the tree nodes:
** token: the token of the node
** left: the left child of the node
** right: the right child of the node
*/
typedef struct	s_ast
{
	t_node_type			type;
	t_command			*command;
	t_node_dir			direction;
	bool				piped;
	struct s_ast		*left;
	struct s_ast		*right;
	t_error				error;
}						t_ast;

/*
** t_ast: struct for the abstract syntax tree:
** graphical representation of the tree: cmd1 a > b > c | cmd2 d < e
** 		      PIPE					  {node: pipe}
** 		     /    	\				  
** 		 COMMAND  	  COMMAND		  {node: command}
** 	   / /  |  \  	  /  |    \
** cmd1 a > b > c 	cmd2 d  <  e	  {same node command: redirection list}
**
*/

typedef struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct		s_shell
{
	t_ast			*ast;
	t_env			*env;
	int				fd_in;
	int				fd_out;
	int				fd_err;
	int				p_fd[2];
	int				exit_status;
	bool			*g_sigint;
	t_error			error;
}					t_shell;

// // string_utils
// int		ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s1);
// int		ft_putstr_fd(char *s, int fd);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_substr(char const *s, unsigned int start, size_t len);

// // builtins_cmd
int		ft_pwd(t_shell *data);
int		ft_echo(char **args);
int		ft_cd(t_shell *data, char **args);
void	ft_perror(char *name, char *str, char *suffix);
int		nbr_args(char **args);
void	ft_env(char *prefix, t_env *env, bool null_value);
int		ft_unset(char **args, t_shell *data);
void	ft_exit(t_shell *data);
int		ft_export(t_shell *data, char **args);
int		ft_env_addback(t_env **env, char *name, char *value);

// // redirections
int		red_in(char **args, t_shell *data);

// // hanlders
int		dup2_handle(int fd1, int fd2);
int		close_handle(int fd);
int		pipe_handle(int *pipefd);

#endif