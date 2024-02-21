/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:59 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/21 23:42:55 by amajid           ###   ########.fr       */
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
# include <limits.h>
# include <sys/types.h>
# include <dirent.h>
# include "libft.h"
# include "enum.h"
# include "ast_handler.h"
# include "builtins.h"
# include "parser.h"
# include <sys/stat.h>

# define YELLOW "\033[0;33m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define MAGENTA "\033[0;35m"
# define WHITE "\033[0;37m"
# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"
# define BLINK "\033[5m"
# define RESET "\033[0m"

# define ALLOC_ERR "minishell: malloc: Cannot allocate memory\n"

extern unsigned int		g_signal;
typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef struct s_command	t_command;
typedef struct s_redirection	t_redirection;
typedef struct s_token	t_token;
typedef struct s_ast	t_ast;

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
	int					heredoc_fd;
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
	char			**args;
	char			**expanded_args;
	char			**globed_args;
	long			arg_count;
	long			arg_size;
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
	int					fd[2];
	bool				piped;
	struct s_ast		*left;
	struct s_ast		*right;
	t_error				error;
	t_shell				*shell;
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
	bool			local;
	struct s_env	*next;
}					t_env;

typedef struct		s_shell
{
	char			*line;
	t_ast			*ast;
	t_env			*env;
	int				fd_in;
	int				fd_out;
	int				exit_status;
	unsigned int	*g_signal;
	t_error			error;
}					t_shell;


void 			print_args(char **args, char *name);

// // main
void			ft_free_shell(t_shell *shell);

// // string_utils
void			ft_free_args(char **args);


// int		ft_strlen(const char *s);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(const char *s1);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strchr(const char *s, int c);
size_t			ft_strlcpy(char *dest, const char *src, size_t dstsize);
char			*ft_substr(char const *s, unsigned int start, size_t len);

// // redirections
int				red_in(char **args, t_shell *data);

// // hanlders
int				dup2_handle(int fd1, int fd2);
int				close_handle(int fd);
int				pipe_handle(int *pipefd);

// // expansion
char			*ft_expand_arg(t_ast *ast, char *arg);
char			**ft_expand_args(t_ast *ast, char **args);
void			handle_dollar(t_ast *ast, char **arg, char **expanded, bool quoted);

// // glober
char			**ft_glob_args(t_ast *ast, char **args);
char			**ft_strsjoin(char **dst, char **src);
struct dirent	*ft_readdir(DIR *dir);
int				ft_entryjoin(struct dirent *entry, char ***files, char *pattern);
int				match(char *pattern, char *string);
int				glob_asterisk(char ***globed_args, char *args, bool quoted);

// // exec
int				exec_redir(t_ast *ast);
int				exec_args(t_ast *ast);
int				is_builtin(char *cmd);
int				exec_ast(t_ast *ast);
size_t			ft_argslen(char **args);
char			*ft_file_param(t_ast *ast, char *file);
char			*ft_get_path(t_ast *ast, char *cmd, int *status);
void			ft_cmd_nf_err(char *cmd, int status);

int				exec_child(t_ast *ast);
int				exec_parent(t_ast *ast);
void			exec_child_pipe(t_ast *ast, t_node_dir dir, int fd[2]);

#endif