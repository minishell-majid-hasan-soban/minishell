/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:26:58 by amajid            #+#    #+#             */
/*   Updated: 2024/02/29 19:32:43 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_token_arr		t_token_arr;
typedef struct s_token			t_token;
typedef struct s_redirection	t_redirection;
typedef struct s_command		t_command;

typedef struct s_err
{
	t_token_type	past_type;
	t_token_type	type;
	long			index;
	long			is_in_op;
	long			j;
	bool			is_word_found;
	bool			is_after_parantheses;
}	t_err;

typedef struct s_parse_exp
{
	t_ast	*l_ast;
	t_token	*op_token;
	t_ast	*r_ast;
	t_ast	*new_ast;
}	t_parse_exp;

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
int					add_separator(t_token_type type, char **prompt,
						t_token_arr *tokens);
int					add_eof(t_token_arr *tokens);
int					handle_seperator(char **prompt, t_token_arr *tokens);
int					skip_str(char **str, char quote);
int					add_word(char **prompt, t_token_arr *tokens);
t_token_arr			tokenize(char *prompt);
void				free_token_arr(t_token_arr *tokens);
t_ast				*create_ast_node(t_node_type type, t_command *command);
void				free_args(t_command *command);
void				free_redirections(t_redirection *redirections);
void				free_command(t_command *command);
void				free_ast(t_ast *node);
int					add_node_arg(t_command *command, char *arg);
t_redirection		*create_redirection(t_token *token_op, t_token file);
int					add_back_redirection(t_redirection **head,
						t_redirection *redir);
t_redirection_type	get_redirection_type(t_token_type type);
t_ast				*extract_command(t_token **curr_token);
char				token_is_operator(t_token *curr_token);
int					token_precedence(t_token *curr_token);
t_node_type			determine_node_type(t_token *op_token);
t_ast				*parse_expression(t_token **curr_token, int min_precedence,
						bool in_op);
char				*redirection_type_to_string(t_redirection_type type);
void				print_redirections(t_redirection *redirection);
void				print_command_args(t_command *cmd);
void				print_ast(const t_ast *node, const char *prefix,
						int isLeft);
// char				*skip_quotes(char *str);
int					ft_read_here_doc(t_ast *ast, int fd_w, char *limiter,
						bool expand);
int					init_here_doc(t_ast *ast, char *limiter, int *status);
int					check_errors_tokens(t_token_arr *tokens, t_ast *ast);
int					check_errors_part1(t_token_arr *tokens, t_err *d);
int					check_errors_part2(t_token_arr *tokens, t_err *d);
int					check_errors_part3(t_token_arr *tokens, t_ast *ast,
						t_err *d);
int					check_errors_part4(t_token_arr *tokens, t_ast *ast,
						t_err *d);
int					check_errors_part5(t_token_arr *tokens, t_ast *ast,
						t_err *d);
char				*expand_heredoc(t_ast *ast, char *line);

#endif