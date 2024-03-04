/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:45:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 20:47:29 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && (line[i] < 9 || line[i] > 13))
			return (0);
		i++;
	}
	return (1);
}

static void	reset_termios(struct termios og_term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &og_term);
}

int	ft_readline(t_shell *shell)
{
	shell->line = readline(GREEN"minishell> "RESET);
	check_input(shell);
	if (shell->line == NULL)
		(reset_termios(shell->term), ft_putstr_fd("exit\n", 1),
			ft_free_shell(shell));
	if (ft_empty(shell->line))
	{
		free(shell->line);
		shell->line = NULL;
		return (1);
	}
	add_history(shell->line);
	if (shell->error == T_FATAL)
		return (ft_putstr_fd("minishell: fatal error.\n", 2),
			ft_putstr_fd("please restart the shell.\n", 2), 1);
	return (0);
}

t_token_arr	ft_get_token(t_shell *shell)
{
	t_token_arr	tokens;
	t_ast		token_ast;
	int			her_status;

	token_ast.shell = shell;
	tokens = tokenize(shell->line);
	if (tokens.status == -2)
		her_status = -2;
	else
		her_status = check_errors_tokens(&tokens, &token_ast);
	if (her_status == -1 || her_status == -2
		|| her_status == 130 || her_status == 42)
	{
		if (her_status == -1 || her_status == -2)
			shell->exit_status = 258;
		else
			shell->exit_status = 1;
		exit_status(shell->exit_status, true);
		free_token_arr(&tokens);
		tokens = (t_token_arr){0};
		free(shell->line);
	}
	return (tokens);
}

int	run_cmd(t_shell *shell)
{
	if (shell->ast == NULL)
		return (exit_status(0, true));
	signal(SIGQUIT, ft_signal_handler);
	ft_init_ast(&shell->ast, shell, false);
	shell->exit_status = exec_ast(shell->ast);
	signal(SIGQUIT, SIG_IGN);
	set_underscore(shell);
	ft_free_ast(&shell->ast);
	return (exit_status(shell->exit_status, true));
}
