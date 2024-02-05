/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:28:53 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/05 17:42:33 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>

void	handler(int sig, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	printf("Signal %d from %d\n", sig, info->si_pid);
}

int	main(void)
{
	struct sigaction	act;
	
	printf("sa_flags: %d\n", act.sa_flags);
	printf("sa_sigaction add : %p\n", act.sa_sigaction);
	printf("sa_handler add : %p\n", act.sa_handler);
	act.sa_sigaction = &handler;
	printf("sa_sigaction add : %p\n", act.sa_sigaction);
	printf("sa_handler add : %p\n", act.sa_handler);
	act.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &act, NULL);
	printf("PID: %d\n", getpid());
	printf("sa_flags: %d\n", act.sa_flags);
	while (1)
		pause();
}
