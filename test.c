/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 02:42:30 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/26 02:09:26 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>

void	run_cmd(char *cmd, int *in, int *out)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(in[0], 0);
		dup2(out[1], 1);
		
		if (in[0] != 0)
			close(in[0]);
		if (out[1] != 1)
			close(out[1]);
		fprintf(stderr, "cmd : %s in[0] : %d out[1] : %d\n", cmd, in[0], out[1]);
		fflush(stderr);
		execve(cmd, (char *[]){cmd, NULL}, NULL);
		perror(cmd);
		exit(1);
	}

}

int	main(int ac, char **av)
{
	int	i;
	int	fd[2][2];

	int f = open("test.c", O_RDONLY);
	dup2(f, 0);
	close(f);
	i = 1;
	while (i < ac) 
	{
		if (i != 1)
			pipe(fd[0]);
		if (i != ac - 1)
			pipe(fd[1]);
		if (i == 1)
			run_cmd(av[i], (int []){0, 1}, fd[1]);
		else if (i == ac - 1)
			run_cmd(av[i], fd[0], (int []){1, 0});
		else
			run_cmd(av[i], fd[0], fd[1]);
		if (i != 1)
		{
			close(fd[0][0]);
			close(fd[0][1]);
		}
		if (i != ac - 1)
		{
			close(fd[1][0]);
			close(fd[1][1]);
		}
		i++;
	}
	i = 0;
	while (i < ac - 1)
	{
		wait(NULL);
		i++;
	}
	
}
