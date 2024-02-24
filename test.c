/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/24 17:10:48 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"
#include "minishell.h"

unsigned int	g_signal;


int	exitstatus(int newstatus, int flag)
{
	static int status;

	if (flag == 1)
		status = newstatus;
	return (status);
}

void print_args(char **args, char *name)
{
	int i;

	i = 0;
	printf("%s\n", name);
	while (args[i])
	{
		printf("args[%d]: %s\n", i, args[i]);
		i++;
	}
}

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

void sig_()
{
	g_signal |= 2;
}

static void	ft_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		// printf("flag %d", g_signal & 1);
		if ((g_signal && 1) == 0)
		{
			// if ((g_signal & 2) == 0)
			sig_();
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (signum == SIGQUIT)
	{
		if (g_signal & 1 && (g_signal & 4) == 0)
		{
			ft_putstr_fd("Quit: 3\n", 1);
			// rl_on_new_line();
			// rl_replace_line("", 0);
			// rl_redisplay();
			g_signal |= 4;
		}
	}
}
// static void ft_sigquit_handler(int signum)
// {
// 	if (signum == SIGQUIT)
// 	{
// 		if (g_signal & 1)
// 		{
// 			ft_putstr_fd("Quit: 3\n", 1);
// 			g_signal |= 4;
// 		}
// 	}
// }

static t_shell	*ft_init_shell(t_shell *shell, char **envp, t_ast *ast)
{
	(void)ast;
	
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
	ft_add_env(ast, ft_strdup("OLDPWD"));
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
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
	ft_free_args(cmd->args);
	ft_free_args(cmd->expanded_args);
	ft_free_args(cmd->globed_args);
	redir = cmd->redirections;
	while (redir)
	{
		tmp = redir;
		if (redir->type == R_HEREDOC)
			if (redir->heredoc_fd != -1 && close(redir->heredoc_fd) == -1)
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

void	ft_free_shell(t_shell *shell)
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

int	ft_init_ast(t_ast **ast, t_shell *shell, bool piped)
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
	return (status);
}

int main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_ast		*ast;
	t_token_arr tokens;
	t_ast		token_ast;
	int			her_status;
	struct termios og_term;
	struct termios md_term;
	
	(void)argc;
	(void)argv;
	token_ast.shell = &shell;
	ft_init_shell(&shell, envp, &token_ast);
	tcgetattr(STDIN_FILENO, &og_term);
	md_term = og_term;
	md_term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &md_term);
	while (1)
	{
		shell.line = readline(GREEN"minishell> "RESET);
		if (shell.line == NULL)
			(tcsetattr(STDIN_FILENO, TCSANOW, &og_term),
				ft_putstr_fd("exit\n", 1), ft_free_shell(&shell));
		if (ft_strlen(shell.line) == 0)
		{
			free(shell.line);
			continue ;
		}
		add_history(shell.line);
		g_signal |= 1;
		tokens = tokenize(shell.line);
		her_status = check_errors_tokens(&tokens, &token_ast);
		if (her_status == -1 || her_status == 130 || her_status == 131)
		{
			if (her_status == -1)
				shell.exit_status = 2;
			else
				shell.exit_status = her_status;
			free_token_arr(&tokens);
			tokens = (t_token_arr){NULL, 0, 0};
		}
		ast = parse_expression(&tokens.arr, 1, false);
		// print_ast(ast, " ", 0);
		if (ast)
		{
			ft_init_ast(&ast, &shell, false);
			signal(SIGQUIT, ft_signal_handler);
			shell.exit_status = exec_ast(ast);
			if (g_signal & 2)
			{
				shell.exit_status = 130;
			}
			else if (g_signal & 4)
			{
				shell.exit_status = 131;
			}
			signal(SIGQUIT, SIG_IGN);
			ft_free_ast(&ast);
		}
		g_signal = 0;
		free(shell.line);
		(dup2(shell.fd_in, 0), dup2(shell.fd_out, 1));
	}
	return (0);
}


