/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   istty.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 04:39:50 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/28 02:53:10 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {

	close(3);
	perror("close");
	dup2(1, 3);
	perror("dup2");
    return 0;
}


