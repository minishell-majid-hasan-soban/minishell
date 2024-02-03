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

t_token_arr tokenize(char *prompt)
{
	t_token_arr res;

	res = ft_realloc(res.arr, res.size, size_t new_size)
}



int main() {

}
