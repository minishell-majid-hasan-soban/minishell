/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:00:57 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/01 12:16:52 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_nodes(t_ast *ast)
{
	int	count;

	count = 0;
	if (ast == NULL)
		return (0);
	count += count_nodes(ast->left);
	count += count_nodes(ast->right);
	return (count + 1);
}

static int	empty_underscore(t_shell *shell)
{
	char	*arg;
	char	*underscore;

	arg = ft_strdup("");
	if (arg == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	underscore = ft_strdup("_");
	if (underscore == NULL)
		return (free(arg), ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (ft_setenv(shell, underscore, arg, false));
}

int	set_underscore(t_shell *shell)
{
	int		count;
	char	*arg;
	char	*under;
	char	**args;

	count = count_nodes(shell->ast);
	if (count != 1)
		return (empty_underscore(shell));
	args = shell->ast->command->globed_args;
	if (args == NULL || *args == NULL)
		return (empty_underscore(shell));
	arg = ft_strdup(args[ft_argslen(args) - 1]);
	if (arg == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), 1);
	under = ft_strdup("_");
	if (under == NULL)
		return (free(arg), ft_putstr_fd(ALLOC_ERR, 2), 1);
	return (ft_setenv(shell, under, arg, false));
}
