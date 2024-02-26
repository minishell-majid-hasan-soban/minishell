/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:46:04 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/26 10:46:08 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_init_ast(t_ast **ast, t_shell *shell, bool piped)
{
	int		status;
	
	status = 0;
	if (*ast == NULL)
		return (0);
	if ((*ast)->type == N_PIPE)
		piped = true;
	else
		piped = false;
	if (ft_init_ast(&(*ast)->left, shell, piped)
		|| ft_init_ast(&(*ast)->right, shell, piped))
		return (1);
	(*ast)->shell = shell;
	(*ast)->piped = piped;
	(*ast)->error = T_NONE;
	return (status);
}
