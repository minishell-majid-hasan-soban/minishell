/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 15:55:12 by hsobane          ###   ########.fr       */
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

static void	reset_shell(t_shell *shell)
{
	free(shell->line);
	if (dup2(shell->fd_in, 0) == -1)
		ft_putstr_fd("dup2 failed to reset stdin\n", 2);
	if (dup2(shell->fd_out, 1) == -1)
		ft_putstr_fd("dup2 failed to reset stdout\n", 2);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	t_token_arr		tokens;
	t_token_arr		tokens_tmp;

	(void)argc;
	(void)argv;
	ft_init_shell(&shell, envp);
	while (1)
	{
		if (ft_readline(&shell))
			continue ;
		ast_running(true, true);
		tokens = ft_get_token(&shell);
		if (!tokens.arr)
			continue ;
		tokens_tmp = tokens;
		shell.ast = parse_expression(&tokens_tmp.arr, 1, false);
		free_token_arr(&tokens);
		run_cmd(&shell);
		ast_running(false, true);
		if (!shell.tty)
			ft_free_shell(&shell);
		reset_shell(&shell);
	}
	return (0);
}
