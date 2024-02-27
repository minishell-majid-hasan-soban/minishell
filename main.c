/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/27 11:10:44 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libc.h>

int main(int argc, char *argv[]) {
	int	i = 150;
	pid_t	pid;

	while (i--)
		pid = fork();
	
}

