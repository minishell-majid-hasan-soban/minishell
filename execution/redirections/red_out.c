/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_out.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:22:00 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/16 08:46:48 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	red_out(char **args, t_shell *data)
// {
// 	int		fd;

// 	if (!args || !*args)
// 		return (ft_perror(data->name, "", "parse error"), -1);
// 	fd = open(args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (ft_perror(data->name, "", args[0]), -1);
// 	if (dup2_handle(fd, 1) < 0)
// 		return (ft_perror(data->name, "", args[0]), -1);
// 	return (0);
// }
