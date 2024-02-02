#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pars.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (new_size == 0 && ptr != NULL)
	{
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return malloc(new_size);
    void *new_ptr = malloc(new_size);
    if (new_ptr == NULL)
		return NULL;
	size_t bytes_to_copy = (old_size < new_size) * old_size + !(old_size < new_size) * new_size;
    ft_memcpy(new_ptr, ptr, bytes_to_copy);
    free(ptr);
   return new_ptr;
}

char **add_pipes(char **command)
{
	int i = 1;
	while(command[i - 1])
		i++;
	
	command = ft_realloc(command, i * sizeof(char *), i * sizeof(char *) + (i - 1) * sizeof(char *));
	int j = 1;
	int a = 1;
	while(j < (i + i - 3))
	{
		ft_memmove(command + j + 1, command + j, (i - a) * sizeof(char *));
		command[j] = malloc(sizeof(char) * 2);
		command[j][0] = '|';
		command[j][1] = 0;
		a++;
		j+=2;
	}
	return command;
}

void replace_char_in_strings(char* str, const char c, const char rep) {
    bool in_single_quote = false;
    bool in_double_quote = false;
    char previous_char = 0;
    int i = 0;
    // Iterate through each character in the string
    while (str[i]) {
        if (str[i] == '\'' && previous_char != '\\') {
            // Toggle the single quote flag if not escaped
            in_single_quote = !in_single_quote;
        } else if (str[i] == '\"' && previous_char != '\\') {
            // Toggle the double quote flag if not escaped
            in_double_quote = !in_double_quote;
        } else if ((in_single_quote || in_double_quote) && str[i] == c) {
            // If in quotes and found the character, mark it as '\1'
            str[i] = rep;
        }
        previous_char = str[i];
		i++;
    }
}


void free_double_ptr_array(char **arr) {
    if (!arr) return;
    int i = 0;
    while (arr[i] != NULL) {
        free(arr[i]);
        i++;
    }
    free(arr);
}

t_token new_token(int type, char *value) {
    t_token token;
    token.type = type;
    token.value = ft_strdup(value); // Duplicate the string to ensure safety.
    // Note: Ensure this strdup memory is managed properly to avoid leaks.
    return token;
}

void print_double_ptr(char **command)
{
	int i = -1;
	while(command[++i])
	{
		printf("%s\n", command[i]);
	}
	printf("---------------------------------------------\n");
}

t_token *process_command(char *commands) {
    int capacity = 10;  // Initial capacity
    int n_tokens = 0;   // Number of tokens
    t_token *tokens = (t_token *)malloc(capacity * sizeof(t_token));

	replace_char_in_strings(commands, '|', -1);
    char **pipe_tokens = ft_split(commands, '|');
	print_double_ptr(pipe_tokens);
    pipe_tokens = add_pipes(pipe_tokens);
	print_double_ptr(pipe_tokens);
	int i = 0;
    while (pipe_tokens[i] != NULL) {
        replace_char_in_strings(pipe_tokens[i], -1, '|');
		replace_char_in_strings(pipe_tokens[i], ' ', -1);	
		char **space_tokens = ft_split(pipe_tokens[i], ' ');

        int j = 0;
        while (space_tokens[j] != NULL) {
            if (n_tokens >= capacity){
                capacity *= 2; // Double the capacity
                tokens = (t_token *)realloc(tokens, capacity * sizeof(t_token));
            }

			replace_char_in_strings(space_tokens[j], -1, ' ');	
            // Here, only space tokens are processed and stored.
            if (j == 0) { // First token is the command.
                tokens[n_tokens++] = new_token(TOKEN_COMMAND, space_tokens[j]);
            } else {     // Following tokens are arguments.
                tokens[n_tokens++] = new_token(TOKEN_ARGUMENT, space_tokens[j]);
            }
            j++;
        }

        free_double_ptr_array(space_tokens); // Cleanup space tokens.
        i++;
    }

    free_double_ptr_array(pipe_tokens); // Cleanup pipe tokens.

    // Resize the token array to the exact number of tokens + 1 for a NULL termination as a safety measure.
    tokens = (t_token *)realloc(tokens, (n_tokens + 1) * sizeof(t_token));
    tokens[n_tokens].type = -1; // Mark the end with a special type. No NULL termination needed for non-pointer structs.
    tokens[n_tokens].value = NULL; // Ensures a clear end if checking value.

    return tokens;
}

int main() {
	char commands[] = "ls -l | grep 'txt | test' | wc -l | ";
    t_token *tokens = process_command(commands);

    int i = 0;
    while (tokens[i].type != -1) { // Use the end marker for termination condition
        printf("Token Type: %d, Value: %s\n", tokens[i].type, tokens[i].value);
        free(tokens[i].value); // Free the duplicated string
        i++;
    }

    free(tokens); // Finally, free the array of tokens.

    return 0;
}
