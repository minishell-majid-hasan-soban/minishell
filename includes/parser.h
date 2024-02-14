/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:26:58 by amajid            #+#    #+#             */
/*   Updated: 2024/02/11 21:27:08 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <malloc/_malloc.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include "../includes/minishell.h"

void				*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void				print_parse_error_near(t_token *token);
void				print_token(t_token *token);
int					is_space(char c);
int					is_str(char c);
int					is_separator(char *str);
int					is_separator_space(char *str);
int					is_in_str(char *str, char c);
void				skip_space(char **prompt);
int					add_token_word(t_token_arr *tokens, char *value);
int					add_separator(t_token_type type, char **prompt, t_token_arr *tokens);
int					add_eof(t_token_arr *tokens);
int					handle_seperator(char **prompt, t_token_arr *tokens);
int					skip_str(char **str, char quote);
int					add_word(char **prompt, t_token_arr *tokens);
t_token_arr			tokenize(char *prompt);
void				free_token_arr(t_token_arr *tokens);
t_ast				*create_ast_node(t_node_type type, t_command* command);
void				free_args(t_command *command);
void				free_redirections(t_redirection *redirections);
void				free_command(t_command *command);
void				free_ast(t_ast* node);
int					add_node_arg(t_command *command, char *arg);
t_redirection		*create_redirection(t_redirection_type type, t_token file);
int					add_back_redirection(t_command *command, t_redirection *redir);
t_redirection_type	get_redirection_type(t_token_type type);
t_ast				*extract_command(t_token** curr_token);
char				token_is_operator(t_token *curr_token);
int					token_precedence(t_token *curr_token);
t_node_type			determine_node_type(t_token *op_token);
t_ast*				parse_expression(t_token** curr_token, int min_precedence, char is_in_op);
char*				redirection_type_to_string(t_redirection_type type);
void				print_redirections(t_redirection* redirection);
void				print_command_args(t_command* cmd);
void				print_ast(const t_ast* node, const char* prefix, int isLeft);

#endif