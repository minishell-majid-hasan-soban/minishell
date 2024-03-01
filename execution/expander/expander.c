/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:55:17 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/01 09:44:41 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(t_ast *ast, char **arg, char **expanded)
{
	char	*to_free;
	char	*tmp;
	int		size;

	if (!arg)
		return ;
	size = ft_argslen(expanded);
	ft_strdupif_zero(ast, expanded, &size);
	tmp = expanded[size - 1];
	to_free = ft_substr(*arg, 0, 1);
	expanded[size - 1] = ft_strjoin(expanded[size - 1], to_free);
	if (!expanded[size - 1])
		ast->error = T_MALLOC;
	(free(tmp), free(to_free));
	(*arg)++;
}

static void	append_squote(t_ast *ast, char **arg, char **expanded)
{
	int		i;
	char	*tmp;
	char	*to_free;
	int		size;

	i = 0;
	(*arg)++;
	while ((*arg)[i] && (*arg)[i] != '\'')
		i++;
	size = ft_argslen(expanded);
	ft_strdupif_zero(ast, expanded, &size);
	tmp = expanded[size - 1];
	to_free = ft_substr(*arg, 0, i);
	expanded[size - 1] = ft_strjoin(expanded[size - 1], to_free);
	(free(tmp), free(to_free));
	if (!expanded[size - 1])
		ast->error = T_MALLOC;
	(*arg) += i + ((*arg)[i] != '\0');
	if (ast->error != T_NONE)
		ft_free_args(expanded);
}

static void	append_dquote(t_ast *ast, char **arg, char **expanded)
{
	char	**tmp;
	char	*join;
	char	*to_free;
	int		size;

	(*arg)++;
	tmp = ft_calloc(2, sizeof(char *));
	while (**arg && **arg != '\"')
	{
		if (**arg == '$')
			handle_dollar(ast, arg, &tmp, true);
		else
			append_char(ast, arg, tmp);
	}
	join = ft_strjoin_arr(ast, tmp);
	size = ft_argslen(expanded);
	ft_strdupif_zero(ast, expanded, &size);
	to_free = expanded[size - 1];
	expanded[size - 1] = ft_strjoin(expanded[size - 1], join);
	(free(to_free), free(join));
	if (!expanded[size - 1])
		ast->error = T_MALLOC;
	(*arg) += **arg != '\0';
	if (ast->error != T_NONE)
		ft_free_args(expanded);
}

char	**ft_expand_arg(t_ast *ast, char *arg)
{
	char	**expanded;

	if (!arg)
		return (NULL);
	expanded = ft_calloc(2, sizeof(char *));
	while (*arg)
	{
		if (*arg == '\'')
			append_squote(ast, &arg, expanded);
		else if (*arg == '\"')
			append_dquote(ast, &arg, expanded);
		else if (*arg == '$')
			handle_dollar(ast, &arg, &expanded, false);
		else
			append_char(ast, &arg, expanded);
		if (ast->error == T_MALLOC)
			return (ft_free_args(expanded), ft_putstr_fd(ALLOC_ERR, 2), NULL);
	}
	return (expanded);
}

char	**ft_expand_args(t_ast *ast, char **args)
{
	char	**expanded;
	char	**tmp2;
	char	**tmp;
	int		i;

	if (!args)
		return (NULL);
	i = 0;
	expanded = ft_calloc(2, sizeof(char *));
	while (args && args[i])
	{
		tmp = ft_expand_arg(ast, args[i]);
		if (ast->error == T_MALLOC)
			return (ft_free_args(expanded), NULL);
		tmp2 = expanded;
		expanded = ft_strsjoin(expanded, tmp);
		(free(tmp2), free(tmp));
		i++;
	}
	return (expanded);
}
