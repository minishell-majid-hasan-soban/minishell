/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 03:23:36 by hsobane           #+#    #+#             */
/*   Updated: 2024/01/25 03:25:40 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <libc.h>

int main()
{
	DIR *dir;
	struct dirent *sd;

	dir = opendir("89");
	perror("opendir");
	if (dir == NULL)
	{
		printf("Error! unable to open directory.\n");
		exit(1);
	}
	while ((sd = readdir(dir)) != NULL)
		printf(">> %s\n", sd->d_name);
	closedir(dir);
	return (0);
}