/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 09:01:09 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/04 09:19:50 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

typedef enum	e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_GREAT,
	TOKEN_LESS,
	TOKEN_DGREAT,
	TOKEN_DLESS,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NEWLINE,
	TOKEN_EOF
}				t_token_type;

/*
** t_token_error: enum for token errors:
** T_ERROR_NONE: no error
** T_SYNTAX: syntax error like > at the end of the line, Example: echo >
*/
typedef enum t_error
{
	T_NONE,
	T_SYNTAX,
	T_MALLOC
	
}				t_error;

/*
** t_node_type: enum for node types: node of the tree (ast)
** NODE_COMMAND: a command node can be a simple command or a command with arguments + redirections
** NODE_PIPE: a pipe node, a node with a pipe token
** NODE_AND: a node with an and token
** NODE_OR: a node with an or token
*/
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
}				t_node_type;

/*
** t_redirection_type: enum for redirection types:
** R_INPUT: input redirection, like "echo < file"
** R_OUTPUT: output redirection, like "echo > file"
** R_APPEND: append redirection, like "echo >> file"
** R_HEREDOC: heredoc redirection, like "echo << file"
*/
typedef enum e_redirection_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}				t_redirection_type;

/*
** t_builtin: enum for builtin commands:
** B_NONE: no builtin command
*/
typedef enum e_builtin
{
	B_NONE,
	B_ECHO,
	B_CD,
	B_PWD,
	B_EXPORT,
	B_UNSET,
	B_ENV,
	B_EXIT
}				t_builtin;

typedef enum e_node_dir
{
	N_ROOT,
	N_LEFT,
	N_RIGHT
}				t_node_dir;


#endif