/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:45:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 17:18:21 by hsobane          ###   ########.fr       */
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
	if (isatty(STDIN_FILENO) == 0)
		shell->tty = false;
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
	return (0);
}

t_token_arr	ft_get_token(t_shell *shell)
{
	t_token_arr	tokens;
	t_ast		token_ast;
	int			her_status;

	token_ast.shell = shell;
	tokens = tokenize(shell->line);
	her_status = check_errors_tokens(&tokens, &token_ast);
	if (her_status == -1 || her_status == 130 || her_status == 42)
	{
		if (her_status == -1)
			shell->exit_status = 2;
		else
			shell->exit_status = 1;
		free_token_arr(&tokens);
		tokens = (t_token_arr){0};
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
	set_underscore(shell);
	signal(SIGQUIT, SIG_IGN);
	ft_free_ast(&shell->ast);
	return (exit_status(shell->exit_status, true));
}
