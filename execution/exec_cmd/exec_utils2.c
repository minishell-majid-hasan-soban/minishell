/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 11:09:40 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/03 11:09:47 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_null_args(t_ast *ast)
{
	char	**exp;
	char	**args;
	char	**raw;

	raw = ast->command->args;
	exp = ast->command->expanded_args;
	args = ast->command->globed_args;
	while (*args && ft_strcmp(*args, "") == 0
		&& !strchr(*raw, '\'') && !strchr(*raw, '\"'))
	{
		args++;
		exp++;
		raw++;
	}
	if (!*args || !*exp)
		if (check_for_quotes(ast->command->args))
			return (ft_cmd_nf_err(ast->command->expanded_args[0], 127), 127);
	if (*args && **args == '\0')
		return (ft_cmd_nf_err(ast->command->expanded_args[0], 127), 127);
	return (0);
}
