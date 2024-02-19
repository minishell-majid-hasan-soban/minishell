/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glober.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:16:55 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/19 11:18:26 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int match(char *pattern, char *string)
{
    while (*pattern && *string)
	{
        if (*pattern == '*')
		{
            while (*pattern == '*')
                pattern++;
            if (*pattern == '\0')
                return 1;
            while (*string && *string != *pattern)
                string++;
        } 
		else if (*pattern == *string)
		{
            pattern++;
            string++;
        }
		else
            return 0;
    }
    while (*pattern == '*')
        pattern++;
    return (*pattern == '\0' && *string == '\0');
}

char	**glober(t_ast *ast, char **args)
{
	DIR				*dir;
    struct dirent	*entry;

    // Open the directory
    if ((dir = opendir(directory)) == NULL) {
        perror("Error opening directory");
        return;
    }

    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Use the custom match function to check if the entry matches the pattern
        if (match(pattern, entry->d_name)) {
            printf("Matched file: %s\n", entry->d_name);
        }
    }

    // Close the directory
    closedir(dir);
}
