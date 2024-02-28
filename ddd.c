/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 10:16:50 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/28 11:02:12 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <libc.h>

// ((ls < a | cat > b) < d < e && (cat >> t >> e) < a < b << r || echo fffff ) < a << e

int main()
{
	int i = 200;
	pid_t pid;

	while (i > 0)
	{
		pid = fork();
		i--;
	}
}