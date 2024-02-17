/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/17 17:04:35 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>
#include <stdio.h>
#include <errno.h>

void	f()
{
	system("leaks minishell");
}

void t(char *str)
{
	printf("%s\n", str);
}

int main()
{
	pid_t pid;
	int status;

	// dup 0 to 3 and 1 to 4
	dup2(0, 3);
	dup2(1, 4);
}
