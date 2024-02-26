/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 14:21:44 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_running(bool flag, bool set)
{
	static bool	executed;

	if (set)
		executed = flag;
	return (executed);
}

int	exit_status(int newstatus, bool set)
{
	static int	status;

	if (set)
		status = newstatus;
	return (status);
}

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

int main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_ast		*ast;
	t_token_arr tokens;
	t_ast		token_ast;
	int			her_status;
	
	(void)argc;
	(void)argv;
	token_ast.shell = &shell;
	ft_init_shell(&shell, envp, &token_ast);
	while (1)
	{
		if (ft_readline(&shell))
			continue ;
		ast_running(true, true);
		tokens = ft_get_token(&shell);
		shell.ast = parse_expression(&tokens.arr, 1, false);
		free_token_arr(&tokens);
		run_cmd(&shell);
		ast_running(false, true);
		ast = NULL;
		if (isatty(0) == 0)
			break ;
		free(shell.line);
		(dup2(shell.fd_in, 0), dup2(shell.fd_out, 1));
	}
	return (0);
}


