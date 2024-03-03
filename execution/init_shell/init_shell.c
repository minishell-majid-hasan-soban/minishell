/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:37:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/02 15:29:00 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct termios	ft_settermios(void)
{
	struct termios	og_term;
	struct termios	md_term;

	tcgetattr(STDIN_FILENO, &og_term);
	md_term = og_term;
	md_term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &md_term);
	return (og_term);
}

static int	ft_env_to_list(t_env **env, char **envp)
{
	int		i;
	char	*name;
	char	*value;

	i = -1;
	*env = NULL;
	while (envp[++i])
	{
		name = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		if (!name)
			return (ft_putstr_fd(ALLOC_ERR, 2), 1);
		value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (!value)
			return (free(name), ft_putstr_fd(ALLOC_ERR, 2), 1);
		if (ft_env_addback(env, name, value))
			return (free(name), free(value), ft_putstr_fd(ALLOC_ERR, 2), 1);
	}
	return (0);
}

t_shell	*ft_init_shell(t_shell *shell, char **envp)
{
	int	status;

	status = 0;
	shell->exit_status = 0;
	shell->error = T_NONE;
	shell->tty = true;
	status = ft_env_to_list(&shell->env, envp);
	shell->exit_status = status || shell->exit_status;
	status = ft_set_minimal_env(shell);
	shell->exit_status = status || shell->exit_status;
	shell->line = NULL;
	shell->fd_in = dup(0);
	shell->fd_out = dup(1);
	if (shell->fd_in == -1 || shell->fd_out == -1)
	{
		(ft_putstr_fd("minishell: dup: ", 2), perror(""));
		shell->error = T_FATAL;
		shell->exit_status = 42;
	}
	shell->ast = NULL;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	shell->term = ft_settermios();
	return (shell);
}
