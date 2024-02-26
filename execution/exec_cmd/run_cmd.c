/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:45:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 14:32:52 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void reset_termios(struct termios og_term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &og_term);
}

int	ft_readline(t_shell *shell)
{
	shell->line = readline(GREEN"minishell> "RESET);
	if (shell->line == NULL)
		(reset_termios(shell->term), ft_putstr_fd("exit\n", 1),
		ft_free_shell(shell));
	if (ft_strlen(shell->line) == 0)
	{
		free(shell->line);
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
	if (her_status == -1 || her_status == 130)
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

int	count_nodes(t_ast *ast)
{
	int	count;

	count = 0;
	if (ast == NULL)
		return (0);
	count += count_nodes(ast->left);
	count += count_nodes(ast->right);
	return (count + 1);
}

int	run_cmd(t_shell *shell)
{
	if (shell->ast == NULL)
		return (exit_status(0, true));
	signal(SIGQUIT, ft_signal_handler);
	ft_init_ast(&shell->ast, shell, false);
	shell->exit_status = exec_ast(shell->ast);
	signal(SIGQUIT, SIG_IGN);
	ft_free_ast(&shell->ast);
	return (exit_status(shell->exit_status, true));
}
