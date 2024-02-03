/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 02:18:35 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/25 02:46:39 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <libc.h>

int main()
{
	int fd[2];
	int b;
	char buf[PIPE_BUF + 1];

	pipe(fd);
	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execlp("ls", "ls", "-l", NULL);
	}
	else
	{
		close(fd[1]);
		while ((b = read(fd[0], buf, PIPE_BUF)) > 0)
		{
			buf[b] = '\0';
			printf("%s", buf);
		}
	}
}