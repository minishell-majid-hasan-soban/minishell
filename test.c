/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/16 12:19:25 by hsobane          ###   ########.fr       */
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
	ft_free_args(cmd->args, -1);
	ft_free_args(cmd->expanded_args, -1);
	redir = cmd->redirections;
	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->file);
		free(tmp->expanded_file);
		free(tmp);
	}
	free(cmd);
}

static void	ft_free_ast(t_ast **ast)
{
	if (ast == NULL)
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
		if (ft_strlen(shell.line) > 0)
			add_history(shell.line);
		tokens = tokenize(shell.line);
		if(tokens.arr == NULL || tokens.size == 0 || tokens.count == 0)
			return (ft_free_shell(&shell), 0);
		ast = parse_expression(&tokens.arr, 1, false);
		if (ast)
		{
			shell.exit_status = exec_ast(ast);
			ft_free_ast(&ast);
		}
		free(shell.line);
	}
	return (0);
}
