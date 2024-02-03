/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:27:28 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/27 23:55:42 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_perror(char *name, char *str, char *suffix)
{
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	if (errno != 0)
		ft_putstr_fd(strerror(errno), 2);
	if (suffix)
		ft_putstr_fd(suffix, 2);
	ft_putstr_fd("\n", 2);
}
