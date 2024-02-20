/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 16:53:49 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	g_signal;

static void	ft_env_to_list(t_env **env, char **envp)
{
	int		i;
	char	*name;
	char	*value;

	i = 0;
	*env = NULL;
	while (envp[i])
	{
		name = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (!name || !value)
			(free(name), free(value), perror("minishell: malloc: "), exit(1));
		if (ft_env_addback(env, name, value))
			(free(name), free(value), perror("minishell: malloc: "), exit(1));
		i++;
	}
}

static void	ft_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		if (g_signal == 0)
			ft_putstr_fd("Quit: 3\n", 1);
		else
			ft_putstr_fd("Quit: 3\n", 1);
		g_signal = 1;
	}
}

static t_shell	*ft_init_shell(t_shell *shell, char **envp)
{
	ft_env_to_list(&shell->env, envp);
	shell->exit_status = 0;
	shell->line = NULL;
	shell->fd_in = dup(0);
	shell->fd_out = dup(1);
	if (shell->fd_in == -1 || shell->fd_out == -1)
		(perror("dup"), exit(1));
	shell->ast = NULL;
	shell->error = T_NONE;
	shell->g_signal = 0;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, ft_signal_handler);
	return (shell);
}

static void ft_free_env(t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

static void	ft_free_command(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*tmp;

	if (cmd == NULL)
		return ;
	printf("args[0]: %s\n", cmd->args[0]);
	ft_free_args(cmd->args);
	printf("expanded_args[0]: %s\n", cmd->expanded_args[0]);
	ft_free_args(cmd->expanded_args);
	printf("globed_args[0]: %s\n", cmd->globed_args[0]);
	ft_free_args(cmd->globed_args);
	redir = cmd->redirections;
	while (redir)
	{
		tmp = redir;
		if (redir->type == R_HEREDOC)
			if (close(redir->heredoc_fd) == -1)
				ft_putstr_fd("minishell: close: ", 2), perror("");
		redir = redir->next;
		free(tmp->file);
		free(tmp->expanded_file);
		free(tmp);
	}
	free(cmd);
}

static void	ft_free_ast(t_ast **ast)
{
	if (ast == NULL || *ast == NULL)
		return ;
	ft_free_ast(&(*ast)->left);
	ft_free_ast(&(*ast)->right);
	ft_free_command((*ast)->command);
	free(*ast);
	*ast = NULL;
}

static void	ft_free_shell(t_shell *shell)
{
	int	status;
	
	status = shell->exit_status;
	ft_free_env(&shell->env);
	ft_free_ast(&shell->ast);
	free(shell->line);
	if (close(shell->fd_in) || close(shell->fd_out))
		status = 1;
	rl_clear_history();
	exit(status);
}

static void print_redir(t_redirection *redir)
{
	if (redir == NULL)
		return ;
	else if (redir->type == R_INPUT)
		ft_putstr_fd("R_INPUT\n", 1);
	else if (redir->type == R_OUTPUT)
		ft_putstr_fd("R_OUTPUT\n", 1);
	else if (redir->type == R_APPEND)
		ft_putstr_fd("R_APPEND\n", 1);
	else if (redir->type == R_HEREDOC)
		ft_putstr_fd("R_HEREDOC\n", 1);
	else
		ft_putstr_fd("UNKNOWN\n", 1);
}

static void print_node_type(t_ast *ast)
{
	ft_putstr_fd("type: ", 1);
	if (ast->type == N_CMD)
		ft_putstr_fd("COMMAND\n", 1);
	else if (ast->type == N_PIPE)
		ft_putstr_fd("PIPE\n", 1);
	else if (ast->type == N_AND)
		ft_putstr_fd("AND\n", 1);
	else if (ast->type == N_OR)
		ft_putstr_fd("OR\n", 1);
	else
		ft_putstr_fd("UNKNOWN\n", 1);
}

void ft_print_ast(t_ast *ast)
{
	char			**args;
	t_redirection	*redir;

	if (ast == NULL)
		return ;
	ft_print_ast(ast->left);
	ft_print_ast(ast->right);
	print_node_type(ast);
	if (ast->command == NULL)
		return ;
	ft_putstr_fd("command: \n", 1);
	args = ast->command->args;
	ft_putstr_fd("args : \n", 1);
	while (*args)
	{
		ft_putstr_fd("\t", 1);
		ft_putstr_fd(*(args++), 1);
		ft_putstr_fd("\n", 1);
	}
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("redirection: \n", 1);
	redir = ast->command->redirections;
	while (redir)
	{
		ft_putstr_fd("\t", 1);
		ft_putstr_fd(redir->file, 1);
		ft_putstr_fd(" ", 1);
		print_redir(redir);
		redir = redir->next;
	}
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\n", 1);
}
//	((a > b aa aaa > c gg) | (d < e) || (f > g > h)) && (i < j < k && l > m > n) || (o > p > q)

static int	ft_read_here_doc(t_ast *ast, int fd_w, char *file)
{
	char			*line;
	
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (0);
		if (ft_strcmp(line, file) == 0)
		{
			free(line);
			break ;
		}
		(ft_putstr_fd(line, fd_w), ft_putstr_fd("\n", fd_w), free(line));
	}
	ft_close(ast, fd_w);
	return (0);
}

static int	init_here_doc(t_ast *ast)
{
	t_redirection	*redir;
	int				fd_w;

	redir = ast->command->redirections;
	while (redir)
	{
		if (redir->type == R_HEREDOC)
		{
			fd_w = open("/tmp/.minishell_heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd_w == -1)
				return (ft_putstr_fd("minishell: ", 2),
					ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), 1);
			redir->heredoc_fd = open("/tmp/.minishell_heredoc", O_RDONLY);
			if (redir->heredoc_fd == -1)
				return (ft_close(ast, fd_w), ft_putstr_fd("minishell: ", 2),
					ft_putstr_fd(strerror(errno), 2), ft_putstr_fd("\n", 2), 1);
			unlink("/tmp/.minishell_heredoc");
			if (ft_read_here_doc(ast, fd_w, redir->file))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

static int	ft_init_ast(t_ast **ast, t_shell *shell, bool piped)
{
	int		status;
	
	status = 0;
	if (*ast == NULL)
		return (0);
	if ((*ast)->type == N_PIPE)
		piped = true;
	else
		piped = false;
	if (ft_init_ast(&(*ast)->left, shell, piped)
		|| ft_init_ast(&(*ast)->right, shell, piped))
		return (1);
	(*ast)->shell = shell;
	(*ast)->piped = piped;
	(*ast)->error = T_NONE;
	if ((*ast)->type == N_CMD)
		status = init_here_doc(*ast);
	return (status);
}

int main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_ast		*ast;
	t_token_arr tokens;
	
	(void)argc;
	(void)argv;
	ft_init_shell(&shell, envp);
	while (1)
	{
		shell.line = readline("minishell> ");
		if (shell.line == NULL)
			(ft_putstr_fd("exit\n", 1), ft_free_shell(&shell));
		if (ft_strlen(shell.line) == 0)
		{
			free(shell.line);
			continue ;
		}
		add_history(shell.line);
		tokens = tokenize(shell.line);
		if(tokens.arr == NULL || tokens.size == 0 || tokens.count == 0)
			shell.exit_status = 2;
		ast = parse_expression(&tokens.arr, 1, false);
		if (ast)
		{
			// print_ast(ast, " ", 0);
			ft_init_ast(&ast, &shell, false);
			shell.exit_status = exec_ast(ast);
			ft_free_ast(&ast);
		}
		free(shell.line);
		(dup2(shell.fd_in, 0), dup2(shell.fd_out, 1));
	}
	return (0);
}
