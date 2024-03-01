/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsobane <hsobane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 09:01:09 by hsobane           #+#    #+#             */
/*   Updated: 2024/03/01 14:22:23 by hsobane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

typedef enum e_token_type
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
	TOKEN_OP,
	TOKEN_CP,
	TOKEN_EOF
}				t_token_type;

typedef enum t_error
{
	T_NONE,
	T_SYNTAX,
	T_MALLOC,
	T_FUNC,
	T_FATAL
}				t_error;

typedef enum e_node_type
{
	N_CMD,
	N_PIPE,
	N_AND,
	N_OR,
}				t_node_type;

typedef enum e_redirection_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}				t_redirection_type;

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