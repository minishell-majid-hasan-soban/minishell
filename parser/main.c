#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pars.h"

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

// Function to free a double pointer array
void free_double_ptr_array(char **arr) {
    if (!arr) return;
    for (int i = 0; arr[i] != NULL; ++i) {
        free(arr[i]);
    }
    free(arr);
}

// Function to create a new token
t_token *new_token(int type, char *value) {
    t_token *token = (t_token *)malloc(sizeof(t_token));
    if (!token) {
        perror("Failed to allocate memory for a new token");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = value; // Assuming value is already allocated, or static string
    return token;
}

// Function to print a token
void print_token(t_token *token) {
    if (token->type == TOKEN_COMMAND)
        printf("Command: %s\n", token->value);
    else if (token->type == TOKEN_ARGUMENT)
        printf("Argument: %s\n", token->value);
}



int main() {
    char test_str[] = "This is a 'test string' with \"quotes\" and \\\"escaped quotes\\\" more 'tests with c' inside";
    char to_replace = 't';  // Character to replace within quotes.
    
    printf("Original String: %s\n", test_str);
    replace_char_in_strings(test_str, to_replace, -1);
    printf("Modified String: %s\n", test_str);

   
    printf("\n");
   char commands[] = "ls -l | grep 'txt | test' | wc -l\0";
	replace_char_in_strings(commands, '|', -1);
    char **pipe_tokens = ft_split(commands, '|');

    for (int i = 0; pipe_tokens[i] != NULL; ++i) {
        // Each pipe token is considered a command
		replace_char_in_strings(pipe_tokens[i], -1, '|');
        t_token *command_token = new_token(TOKEN_COMMAND, pipe_tokens[i]);
        print_token(command_token);
        free(command_token); // Assume dynamic allocation for demonstration
		
        // Split command into arguments
		replace_char_in_strings(pipe_tokens[i], ' ', -1);
        char **space_tokens = ft_split(pipe_tokens[i], ' ');

        for (int j = 0; space_tokens[j] != NULL; ++j) {
		    // Each space token is considered an argument
			replace_char_in_strings(space_tokens[j], -1, ' ');
            t_token *arg_token = new_token(TOKEN_ARGUMENT, space_tokens[j]);
            print_token(arg_token);
            free(arg_token); // Assume dynamic allocation for demonstration
        }
        
        free_double_ptr_array(space_tokens);
    }

    free_double_ptr_array(pipe_tokens);
    return 0;
}
