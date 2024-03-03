/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:33:16 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/02 15:35:11 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (ast_running(false, false))
		{
			ft_putstr_fd("\n", 1);
			exit_status(130, true);
		}
		else
		{
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			exit_status(1, true);
		}
	}
	else if (signum == SIGQUIT)
	{
		if (ast_running(false, false))
		{
			ft_putstr_fd("^\\Quit: 3\n", 1);
			exit_status(131, true);
		}
	}
}

void	sig_heredoc_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		exit(130);
	}
}
