/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:27:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 14:28:40 by hsobane          ###   ########.fr       */
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

int main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	t_token_arr 	tokens;
	
	(void)argc;
	(void)argv;
	ft_init_shell(&shell, envp);
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
		if (isatty(0) == 0)
			return (ft_free_shell(&shell), 0);
		free(shell.line);
		(dup2(shell.fd_in, 0), dup2(shell.fd_out, 1));
	}
	return (0);
}


