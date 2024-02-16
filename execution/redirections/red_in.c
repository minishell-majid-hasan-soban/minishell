/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:51:00 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/16 08:46:44 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	red_in(char **args, t_shell *data)
// {
// 	int		fd;

// 	if (!args || !*args)
// 		return (ft_perror(data->name, "", "parse error"), -1);
// 	fd = open(args[0], O_RDONLY);
// 	if (fd < 0)
// 		return (ft_perror(data->name, "", args[0]), -1);
// 	if (dup2_handle(fd, 0) < 0)
// 		return (ft_perror(data->name, "", args[0]), -1);
// 	return (0);
// }
