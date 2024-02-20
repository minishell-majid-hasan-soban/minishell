/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:49:44 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/20 16:47:22 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int match(const char *pattern, const char *string) {
    while (*pattern && *string) {
        if (*pattern == '*') {
            // Handle '*' wildcard
            while (*pattern == '*') {
                pattern++;
            }

            // If '*' is the last character in the pattern, it's a match
            if (*pattern == '\0') {
                return 1;
            }

            // Search for the next occurrence of the character after '*'
            while (*string && *string != *pattern) {
                string++;
            }
        } else if (*pattern == *string) {
            // Characters match, move to the next ones
            pattern++;
            string++;
        } else {
            // Characters do not match
            return 0;
        }
    }

    // Check for the case where the pattern ends with '*'
    while (*pattern == '*') {
        pattern++;
    }

    // If both pattern and string reach the end, it's a match
    return (*pattern == '\0' && *string == '\0');
}

void globFiles(const char *pattern, const char *directory) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    if ((dir = opendir(directory)) == NULL) {
        perror("Error opening directory");
        return;
    }
	printf("dir name: %s\n", dir->__dd_buf);

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

int main(int argc, char *argv[]) {
    free(NULL);

    return 0;
}

