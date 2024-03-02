/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 19:40:38 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/02 08:53:09 by hsobane          ###   ########.fr       */
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
# include <dirent.h>
# include "libft.h"
# include "enum.h"
# include "ast_handler.h"
# include "builtins.h"
# include "parser.h"
# include <sys/stat.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <term.h>

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

typedef struct s_shell			t_shell;
typedef struct s_env			t_env;
typedef struct s_command		t_command;
typedef struct s_redirection	t_redirection;
typedef struct s_token			t_token;
typedef struct s_ast			t_ast;

typedef struct s_token
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
	int		status;
}	t_token_arr;

typedef struct s_redirection
{
	t_redirection_type		type;
	char					*file;
	char					*expanded_file;
	int						heredoc_fd;
	bool					expanded;
	struct s_redirection	*next;
	struct s_redirection	*prev;
	t_error					error;
}							t_redirection;

typedef struct s_command
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

typedef struct s_ast
{
	t_node_type			type;
	t_command			*command;
	t_redirection		*redirections;
	t_node_dir			direction;
	int					fd[2];
	bool				piped;
	struct s_ast		*left;
	struct s_ast		*right;
	t_error				error;
	t_shell				*shell;
}						t_ast;

typedef struct s_env
{
	char			*name;
	char			*value;
	bool			local;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	char			*line;
	t_ast			*ast;
	t_env			*env;
	struct termios	term;
	int				fd_in;
	int				fd_out;
	int				exit_status;
	bool			tty;
	t_error			error;
}					t_shell;

// main
void			ft_free_shell(t_shell *shell);
int				exit_status(int newstatus, bool flag);
struct termios	ft_settermios(void);

// string_utils
void			ft_free_args(char **args);

int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(const char *s1);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strchr(const char *s, int c);
size_t			ft_strlcpy(char *dest, const char *src, size_t dstsize);
char			*ft_substr(char const *s, unsigned int start, size_t len);

// parser
char			**ft_skip_args(char **args);
char			*skip_quotes(char *str);

// hanlders
int				dup2_handle(int fd1, int fd2);
int				close_handle(int fd);
int				pipe_handle(int *pipefd);

// expansion
char			**ft_expand_arg(t_ast *ast, char *arg);
char			**ft_expand_args(t_ast *ast, char **args);
void			handle_dollar(t_ast *ast, char **arg, char ***expanded,
					bool quoted);
int				set_underscore(t_shell *shell);
void			append_char(t_ast *ast, char **arg, char **expanded);
char			*handle_question(t_ast *ast, char **arg);
char			*handle_alphanum(t_ast *ast, char **arg, bool quoted);
char			*ft_strjoin_arr(t_ast *ast, char **arr);
void			ft_strdupif_zero(t_ast *ast, char **expanded, int *size);

// glober
char			**ft_glob_args(t_ast *ast, char **args);
char			**ft_strsjoin(char **dst, char **src);
struct dirent	*ft_readdir(DIR *dir);
int				ft_entryjoin(struct dirent *entry, char ***files,
					char *pattern);
int				match(char *pattern, char *string);
int				glob_asterisk(char ***globed_args, char *args, bool quoted);
bool			is_quoted(char *arg, char target, bool all);
int				ft_glob_arg(t_ast *ast, char *arg, char ***globed);

// exec
int				exec_redir(t_ast *ast, t_redirection *redir);
int				exec_args(t_ast *ast);
int				is_builtin(char *cmd);
int				exec_ast(t_ast *ast);
size_t			ft_argslen(char **args);
char			*ft_file_param(t_ast *ast, char *file);
char			*ft_get_path(t_ast *ast, char *cmd, int *status);
void			ft_cmd_nf_err(char *cmd, int status);
int				check_file_name(char *file);

int				exec_child(t_ast *ast);
int				exec_parent(t_ast *ast);
void			exec_child_pipe(t_ast *ast, t_node_dir dir, int fd[2]);

// shell
int				run_cmd(t_shell *shell);
t_token_arr		ft_get_token(t_shell *shell);
int				ft_readline(t_shell *shell);

// free
void			ft_free_args(char **args);
void			ft_free_env(t_env **env);
void			ft_free_command(t_command *cmd);
void			ft_free_ast(t_ast **ast);
void			ft_free_shell(t_shell *shell);

// init
int				ft_init_ast(t_ast **ast, t_shell *shell, bool piped);
int				ft_set_minimal_env(t_shell *shell);
t_shell			*ft_init_shell(t_shell *shell, char **envp);

// signals
void			ft_signal_handler(int signum);
void			sig_heredoc_handler(int signum);

// static
int				ast_running(bool flag, bool set);
int				exit_status(int newstatus, bool set);

#endif