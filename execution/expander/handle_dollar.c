/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:16:12 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 14:01:09 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_question(t_ast *ast, char **arg)
{
	char	*var;

	(void)ast;
	(*arg)++;
	var = ft_itoa(exit_status(0, false));
	return (var);
}

static char	*handle_alphanum(t_ast *ast, char **arg, bool quoted)
{
	char	*var;
	char	*to_free;
	char	*tmp;
	int		i;

	i = 0;
	while ((*arg)[i] && (ft_isalnum((*arg)[i]) || (*arg)[i] == '_'))
		i++;
	tmp = (void *)-1;
	if (i == 0 && !quoted)
		tmp = ft_strdup("");
	else if (i == 0)
		tmp = ft_strdup("$");
	if (tmp == NULL)
		return (ft_putstr_fd(ALLOC_ERR, 2), NULL);
	else if (tmp != (void *)-1)
		return (tmp);
	to_free = ft_substr(*arg, 0, i);
	var = get_value(ast->shell->env, to_free);
	free(to_free);
	*arg += i;
	return (var);
}

static char	**ft_assemble_arr(char	**r1, char **r2)
{
	char	*last;
	char	*new_last;
	int		size;
	char	**arr;

	if (!r1 || !*r1)
		return (ft_free_args(r1), r2);
	else if (!r2 || !*r2)
		return (ft_free_args(r2), r1);
	size = ft_argslen(r1);
	last = r1[size - 1];
	new_last = ft_strjoin(last, r2[0]);
	(free(last), free(r2[0]));
	r2[0] = new_last;
	r1[size - 1] = NULL;
	arr = ft_strsjoin(r1, r2);
	(free(r1), free(r2));
	return (arr);
}

static void	ft_strjoin_last(char ***expanded, char *var)
{
	char	*last;
	char	*new_last;
	int		size;


	size = ft_argslen(*expanded);
	if (size == 0)
	{
		free(*expanded);
		*expanded = ft_calloc(2, sizeof(char *));
		if (!*expanded)
			return ;
		(*expanded)[0] = var;
		return ;
	}
	last = (*expanded)[size - 1];
	new_last = ft_strjoin(last, var);
	(free(last), free(var));
	(*expanded)[size - 1] = new_last;
}

void	handle_dollar(t_ast *ast, char **arg, char ***expanded, bool quoted)
{
	char	*var;
	char	**split;

	(*arg)++;
	if (!expanded || !*expanded)
		return ;
	if (**arg == '\0')
		var = ft_strdup("$");
	else if (**arg == '?')
		var = handle_question(ast, arg);
	else
		var = handle_alphanum(ast, arg, quoted);
	if (quoted)
		ft_strjoin_last(expanded, var);
	else
	{
		split = ft_split(var, ' ');
		print_args(split, "split");
		print_args(*expanded, "expanded");
		*expanded = ft_assemble_arr(*expanded, split);
		free(var);
	}
}
