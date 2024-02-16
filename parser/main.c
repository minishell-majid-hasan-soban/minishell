/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:08:33 by amajid            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/14 15:16:30 by hsobane          ###   ########.fr       */
=======
/*   Updated: 2024/02/14 15:31:09 by amajid           ###   ########.fr       */
>>>>>>> db8972e50116c65dffc70edf0964c60f0d831945
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/parser.h"


// int main() {
// <<<<<<< HEAD
// 	char *str = "| ls";
// =======
// 	char *str = "ls |";
// >>>>>>> db8972e50116c65dffc70edf0964c60f0d831945
// 	// char *str = "(lskjdfkjhsdfjkhdshjds)" ;
// 	//atexit(check);
// 	t_token_arr tokens = tokenize(str);
// 	if(tokens.arr == NULL || tokens.size == 0 || tokens.count == 0)
// 		return 0;
// 	int i = -1;
// 	char types[][100] = 
// 	{
// 	"TOKEN_WORD",
// 	"TOKEN_PIPE",
// 	"TOKEN_GREAT",
// 	"TOKEN_LESS",
// 	"TOKEN_DGREAT",		// >> double greater than, append
// 	"TOKEN_DLESS",		// << double less than, heredoc
// 	"TOKEN_AND",
// 	"TOKEN_OR",
// 	"TOKEN_NEWLINE",
// 	"TOKEN_OP",			// open parenthesis
// 	"TOKEN_CP",			// close parenthesis
// 	"TOKEN_EOF"
// 	};
// 	while (++i < tokens.count)
// 		printf("token type = %s, value = %s\n", types[tokens.arr[i].type], tokens.arr[i].value);
// 	t_token *arr = tokens.arr;
// 	t_ast *ast = parse_expression(&arr, 1, 0);
// 	print_ast(ast, " ", 0);
// 	free_token_arr(&tokens);
// 	free_ast(ast);
// }
