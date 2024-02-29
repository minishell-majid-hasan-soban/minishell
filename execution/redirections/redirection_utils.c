/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:19:19 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/29 20:20:18 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_file_name(char *file)
{
	char	*skiped;

	skiped = skip_quotes(file);
	if (!skiped)
		return (1);
	if (!*skiped)
		return (ft_putstr_fd("minishell: ", 2),
			ft_putstr_fd(": No such file or directory\n", 2), free(skiped), 1);
	free(skiped);
	return (0);
}
