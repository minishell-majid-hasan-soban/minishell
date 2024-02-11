/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amajid <amajid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:08:33 by amajid            #+#    #+#             */
/*   Updated: 2024/02/11 21:46:35 by amajid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"


void	free_command(t_command *command)
{
	free_args(command);
	command->args = NULL;
	free_redirections(command->redirections);
	command->redirections = NULL;
	free(command);
}

void free_ast(t_ast* node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);

	if(node->command)
		free_command(node->command);
    free(node);
}

int add_node_arg(t_command *command, char *arg)
{
	if(command->arg_size == 0)
	{
		command->args = ft_realloc(command->args, 0, sizeof(char *) * 2);
		if(!command->args)
			return -1;
		command->arg_size = 2;
	}
	if(command->arg_count + 1 > command->arg_size)
	{
		command->args = ft_realloc(command->args, command->arg_size * sizeof(char *), command->arg_size * 2 * sizeof(char *));
		if(!command->args)
			return -1;
		command->arg_size *= 2;
	}
	
	command->args[command->arg_count++] = ft_strdup(arg);
	return (1);
}

t_redirection *create_redirection(t_redirection_type type, t_token file)
{
	t_redirection *redir = malloc(sizeof(t_redirection));
	if(!redir)
		return (NULL);
	if(file.type != TOKEN_WORD)
	{
		print_parse_error_near(&file);
		free(redir);
		return (NULL);
	}
	redir->file = strdup(file.value);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

int add_back_redirection(t_command *command, t_redirection *redir)
{
	if(!(command->redirections))
	{
		(command->redirections) = redir;
		return 1;
	}
	t_redirection *curr = command->redirections;

	while(curr->next)
		curr = curr->next;
	curr->next = redir;
	return 1;
}

t_redirection_type get_redirection_type(t_token_type type)
{
	if(type == TOKEN_GREAT)
		return R_OUTPUT;
	if(type == TOKEN_LESS)
		return R_INPUT;
	if(type == TOKEN_DGREAT)
		return R_APPEND;
	return R_HEREDOC;
}


t_ast* parse_expression(t_token** curr_token, int min_precedence, char is_in_op);

t_ast	*extract_command(t_token** curr_token)
{
	t_command *command = malloc(sizeof(t_command));
	*command = (t_command){0};
	if(!command)
		return (NULL);
	if((*curr_token)->type == TOKEN_OP)
	{
		(*curr_token)++;
		t_ast *ast = parse_expression(curr_token, 1, 1);
		if((*curr_token)->type != TOKEN_CP)
		{
			printf("minishell: expected ')'\n");
			free_ast(ast);
			return NULL;
		}
		(*curr_token)++;

		return ast;
	}
	while((*curr_token)->type != TOKEN_AND && (*curr_token)->type != TOKEN_OR && (*curr_token)->type != TOKEN_PIPE && (*curr_token)->type != TOKEN_EOF && (*curr_token)->type != TOKEN_OP && (*curr_token)->type != TOKEN_CP)
	{
		if((*curr_token)->type == TOKEN_WORD)
			add_node_arg(command, (*curr_token)->value);
		else
		{
			t_redirection *redir = create_redirection(get_redirection_type((*curr_token)->type), *((*curr_token)+1));
			if(redir == NULL)
			{
				free_command(command);
				return (NULL);
			}
			add_back_redirection(command, redir);
			(*curr_token)++;
		}
		(*curr_token)++;
	}
	return create_ast_node(N_CMD, command);
}

char token_is_operator(t_token *curr_token)
{
	if((*curr_token).type == TOKEN_AND || (*curr_token).type == TOKEN_OR || (*curr_token).type == TOKEN_PIPE)
		return (1);
	return (0);
}

int token_precedence(t_token *curr_token)
{
	if((*curr_token).type == TOKEN_AND)
		return 1;
	else if((*curr_token).type == TOKEN_OR)
		return 1;
	else if((*curr_token).type == TOKEN_PIPE)
		return 1;
	return 1;
}

t_node_type determine_node_type(t_token *op_token)
{
	if((*op_token).type == TOKEN_AND)
		return N_AND;
	else if((*op_token).type == TOKEN_OR)
		return N_OR;
	else if((*op_token).type == TOKEN_PIPE)
		return N_PIPE;
	return -1;

}


t_ast* parse_expression(t_token** curr_token, int min_precedence, char is_in_op) {
   t_ast* l_ast = extract_command(curr_token);
	if(l_ast == NULL)
		return NULL;
	if((*curr_token)->type == TOKEN_CP && is_in_op)
		return l_ast;
	if((*curr_token)->type != TOKEN_EOF && !token_is_operator(*curr_token))
	{
		if((*curr_token)->type == TOKEN_WORD)
			printf("minishell: parse error near '%s'\n", (*curr_token)->value);
		else 
			print_parse_error_near((*curr_token));
		// print_token((*curr_token));
		free_ast(l_ast);
		return NULL;
	}
    while ((*curr_token)->type != TOKEN_EOF && token_is_operator(*curr_token) && token_precedence(*curr_token) >= min_precedence) {
		t_token* op_token = *curr_token;
        *curr_token = ++(*curr_token); // Move past the operator.

        t_ast* r_ast = parse_expression(curr_token, token_precedence(op_token) + 1, is_in_op);
		if(r_ast == NULL)
		{
			free_ast(l_ast);
			return NULL;
		}
		
        t_ast* new_ast = create_ast_node(determine_node_type(op_token), NULL); // Convert token to a node type.
        new_ast->left = l_ast; // Attach the LHS AST as the left child.
        new_ast->right = r_ast; // Attach the RHS AST as the right child.

        l_ast = new_ast; // The new AST becomes the LHS of the next iteration (if any).
    }
    return l_ast;
}
char* redirection_type_to_string(t_redirection_type type)
{
	if(type == R_INPUT)
		return "R_INPUT";
	if(type == R_OUTPUT)
		return "R_OUTPUT";
	if(type == R_APPEND)
		return "R_APPEND";
	if(type == R_HEREDOC)
		return "R_HEREDOC";
	
	return NULL;
}

void print_redirections(t_redirection* redirection) {
    while (redirection != NULL) {
        printf(" (%s -> %s)", redirection_type_to_string(redirection->type), redirection->file);
        redirection = redirection->next;
    }
    printf("\n");
}

void print_command_args(t_command* cmd) {
    if (cmd && cmd->args) {
        printf("- ");
        for (long i = 0; i < cmd->arg_count; ++i) {
            printf("%s ", cmd->args[i]);
        }
        // Print redirections if present
        //if (cmd->redirections) {
            print_redirections(cmd->redirections);
        //} else {
            printf("\n");
        //}
    } else {
        printf("-Empty Command");
		print_redirections(cmd->redirections);
		printf("\n");
    }
}

void print_ast(const t_ast* node, const char* prefix, int isLeft) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", isLeft ? "|--" : "\\--");

    if (node->type == N_CMD) {
        print_command_args(node->command);
    } else {
        printf("%s\n", node->type == N_PIPE ? "Pipe" : node->type == N_AND ? "AND": "OR");
    }

    char nextPrefix[256];
    sprintf(nextPrefix, "%s%s", prefix, isLeft ? "|   " : "    ");
    
    print_ast(node->left, nextPrefix, 1);
    print_ast(node->right, nextPrefix, 0);
}

void check()
{
	system("leaks a.out");
}

int main() {
	char *str = "ls  ls  | grep thing > test < thing << a >> b || ls || <  ygerfuy | test && ((ls >> test | ls | test && node >> n)) ||  ' ()  test &&  pipe | < > << >> || '";
	// char *str = "(lskjdfkjhsdfjkhdshjds)" ;
	//atexit(check);
	t_token_arr tokens = tokenize(str);
	if(tokens.arr == NULL || tokens.size == 0 || tokens.count == 0)
		return 0;
	int i = -1;
	char types[][100] = 
	{
	"TOKEN_WORD",
	"TOKEN_PIPE",
	"TOKEN_GREAT",
	"TOKEN_LESS",
	"TOKEN_DGREAT",		// >> double greater than, append
	"TOKEN_DLESS",		// << double less than, heredoc
	"TOKEN_AND",
	"TOKEN_OR",
	"TOKEN_NEWLINE",
	"TOKEN_OP",			// open parenthesis
	"TOKEN_CP",			// close parenthesis
	"TOKEN_EOF"
	};
	while (++i < tokens.count)
		printf("token type = %s, value = %s\n", types[tokens.arr[i].type], tokens.arr[i].value);
	t_token *arr = tokens.arr;
	t_ast *ast = parse_expression(&arr, 1, 0);
	print_ast(ast, " ", 0);
	free_token_arr(&tokens);
	free_ast(ast);
}
