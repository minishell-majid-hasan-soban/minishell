/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 16:33:25 by hsobane           #+#    #+#             */
/*   Updated: 2024/02/03 17:02:30 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** t_token_type: enum for token types:
** TOKEN_WORD: word meaning a command or an argument or a file name
*/
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
typedef enum t_token_error
{
	T_ERROR_NONE,
	T_SYNTAX,
	T_MALLOC
	
}				t_token_error;

/*
** t_token: struct for tokens:
** value: the value of the token if it's a word, like "echo" or "file" or "argument"
** type: the type of the token
** error: the error of the token, if TOKEN_NEWLINE is followed by nothing, it's a syntax error
** next: the next token, list easy a sat
** prev: the previous token
*/
typedef struct	s_token
{
	char				*value;
	int					type;
	t_token_error		*error;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

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
** t_redirection: struct for redirections:
** type: the type of the redirection, like R_INPUT or R_OUTPUT [t_redirection_type enum above]
** file: the file of the redirection, like "file" in "echo hello > file"
** heredoc_fd: used if the redirection is a heredoc, it hodls the read end of the pipe
** next: the next redirection
** prev: the previous redirection
*/
typedef struct	s_redirection
{
	t_redirection_type		type;
	char					*file;
	int						heredoc_fd;
	struct	s_redirection	*next;
	struct	s_redirection	*prev;
}							t_redirection;

/*
** t_command: struct for commands:
** name: the name of the command, like "echo" or "ls"
** args: the arguments of the command, like "echo hello" -> "hello" is the argument
** redirections: the redirections of the command, like "echo hello > file" -> "file" is the redirection of type R_OUTPUT
** t_redirection: is above this struct
*/
typedef struct	s_command
{
	char			*name;
	char			**args;
	t_redirection	*redirections;
}				t_command;

/*
** t_node: struct for the tree nodes:
** token: the token of the node
** left: the left child of the node
** right: the right child of the node
*/
typedef struct	s_node
{
	t_node_type			*token;
	t_command			*command;
	struct s_node		*left;
	struct s_node		*right;
}				t_node;

/*
** t_ast: struct for the abstract syntax tree:
** graphical representation of the tree: cmd1 a > b > c | cmd2 d < e
** 		      PIPE
** 		     /      \
** 		 COMMAND    COMMAND
** 		 /  |  \    /  |  \
** 		a > b > c   d  <   e
** 		root: the root of the tree
*/