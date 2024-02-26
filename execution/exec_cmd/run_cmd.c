/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:45:03 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 14:20:58 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	run_cmd(t_shell *shell)
{
	if (shell->ast == NULL)
		return (exit_status(0, true));
	signal(SIGQUIT, ft_signal_handler);
	ft_init_ast(shell->ast, shell, false);
	shell->exit_status = exec_ast(shell->ast);
	signal(SIGQUIT, SIG_IGN);
	ft_free_ast(&shell->ast);
	return (exit_status(shell->exit_status, true));
}
