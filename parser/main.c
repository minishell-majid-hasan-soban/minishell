#include <malloc/_malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../includes/minishell.h"

void free_token_arr(t_token_arr *tokens);



void *ft_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (new_size == 0 && ptr != NULL)
	{
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return malloc(new_size);
    void *new_ptr = ft_calloc(new_size, 1);
    if (new_ptr == NULL)
		return NULL;
	size_t bytes_to_copy = (old_size < new_size) * old_size + !(old_size < new_size) * new_size;
    ft_memcpy(new_ptr, ptr, bytes_to_copy);
    free(ptr);
   return new_ptr;
}

void print_parse_error_near(t_token *token)
{
		char types[][100] = 
	{
	"",
	"|",
	">",
	"<",
	">>",		// >> double greater than, append
	"<<",		// << double less than, heredoc
	"&&",
	"||",
	"NEWLINE",
	"(",			// open parenthesis
	")",			// close parenthesis
	"newline"
	};
	printf("minishell: syntax error near unexpected token '%s'\n", types[token->type]);
}


void print_token(t_token *token)
{
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
	printf("token type = %s, value = %s\n", types[token->type], token->value);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\f' || c == '\r' || c == '\v')
		return (1);
	return (0);
}


int	is_str(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int is_separator(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "&&", 2)
		|| !ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	return 0;
}

int	is_separator_space(char *str)
{
	if (is_space(*str) || is_separator(str))
		return (1);
	return (0);
}

int is_in_str(char *str, char c)
{
	while(*str && *str != c)
		str++;
	return (*str == c);
}

void skip_space(char **prompt)
{
	while(is_space(**prompt))
		((*prompt)++);
}

int add_token_word(t_token_arr *tokens, char *value)
{
	if(tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){value, TOKEN_WORD, T_NONE, NULL, NULL};
	return (1);
}

int add_separator(t_token_type type, char **prompt, t_token_arr *tokens)
{
	if(tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, type, T_NONE, NULL, NULL};
	(*prompt)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT || type == TOKEN_OR || type == TOKEN_AND)
		(*prompt)++;
	if((type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE) && tokens->count == 1)
	{
		print_parse_error_near(&tokens->arr[tokens->count - 1]);
		return -1;
	}
	t_token_type past_token_type = tokens->arr[tokens->count-2].type;
	if((type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE) && (past_token_type == TOKEN_AND || past_token_type == TOKEN_OR || past_token_type == TOKEN_PIPE))
	{
		print_parse_error_near(&tokens->arr[tokens->count - 1]);
		return -1;
	}
	
	return 1;
}

int add_eof(t_token_arr *tokens)
{
	if(tokens->count + 1 > tokens->size)
	{
		tokens->arr = ft_realloc(tokens->arr, tokens->size * sizeof(t_token), tokens->size * 2 * sizeof(t_token));
		if(!tokens->arr)
			return -1;
		tokens->size *= 2;
	}
	tokens->arr[tokens->count++] = (t_token){NULL, TOKEN_EOF, T_NONE, NULL, NULL};
	return 1;
}

int handle_seperator(char **prompt, t_token_arr *tokens)
{
	if (!ft_strncmp(*prompt, "<<", 2))
		return (add_separator(TOKEN_DLESS, prompt, tokens));
	else if (!ft_strncmp(*prompt, ">>", 2))
		return (add_separator(TOKEN_DGREAT, prompt, tokens));
	else if (!ft_strncmp(*prompt, "&&", 2))
		return (add_separator(TOKEN_AND, prompt, tokens));
	else if (!ft_strncmp(*prompt, "||", 2))
		return (add_separator(TOKEN_OR, prompt, tokens));
	else if (!ft_strncmp(*prompt, "|", 1))
		return (add_separator(TOKEN_PIPE, prompt, tokens));
	else if (!ft_strncmp(*prompt, "<", 1))
		return (add_separator(TOKEN_LESS, prompt, tokens));
	else if (!ft_strncmp(*prompt, ">", 1))
		return (add_separator(TOKEN_GREAT, prompt, tokens));
	else if (!ft_strncmp(*prompt, "(", 1))
		return (add_separator(TOKEN_OP, prompt, tokens));
	else if (!ft_strncmp(*prompt, ")", 1))
		return (add_separator(TOKEN_CP, prompt, tokens));
	return -1;
}


int skip_str(char **str, char quote)
{
	(*str)++;
	if(!is_in_str(*str, quote))
		return -1;

	while(**str != quote)
		(*str)++;
	(*str)++;
	return 1;
}

int add_word(char **prompt, t_token_arr *tokens)
{
	char	*prompt_ptr;
	char	*word;

	prompt_ptr = *prompt;
	while (*prompt_ptr && !is_separator_space(prompt_ptr))
		if (is_str(*prompt_ptr))
		{
			if (!skip_str(&prompt_ptr, *prompt_ptr))
				return (-1);
		}
		else
			prompt_ptr++;
	word = ft_substr(*prompt, 0, prompt_ptr - (*prompt));
	if (!word)
		return (0);
	// printf("prompt_ptr - (*prompt) = %ld\n", prompt_ptr - (*prompt));
	*prompt += prompt_ptr - (*prompt);
	return (add_token_word(tokens, word));
}


t_token_arr tokenize(char *prompt)
{
	t_token_arr tokens;
	int			ret;

	tokens = (t_token_arr){0, 1000, 0};
	tokens.arr = ft_realloc(tokens.arr, 0, tokens.size * sizeof(t_token));
	if(!tokens.arr)
	{
		free(tokens.arr);
		tokens = (t_token_arr){0};
		return tokens;
	}
	while(*prompt)
	{
		if(is_space(*prompt))
			skip_space(&prompt);
		if(is_separator(prompt))
		{
			ret = handle_seperator(&prompt, &tokens);
		}
		else
		{
		 	ret = add_word(&prompt, &tokens);

		}
		if(ret == -1)
		{
			free_token_arr(&tokens);
			tokens = (t_token_arr){0};
			return tokens;
		}
	}
	if(tokens.arr[tokens.count - 1].type == TOKEN_AND || tokens.arr[tokens.count - 1].type == TOKEN_OR || tokens.arr[tokens.count - 1].type == TOKEN_PIPE)
	{
		print_parse_error_near(&tokens.arr[tokens.count - 1]);
		free_token_arr(&tokens);
		return (tokens);
	}
	add_eof(&tokens);
	return tokens;
}


void free_token_arr(t_token_arr *tokens)
{
	int i = -1;

	while(++i < tokens->count)
		if(tokens->arr[i].value && tokens->arr[i].type != TOKEN_EOF)
			free(tokens->arr[i].value);
	free(tokens->arr);
	(*tokens) = (t_token_arr){0};
}

t_ast* create_ast_node(t_node_type type, t_command* command) {
    t_ast* node = (t_ast*)malloc(sizeof(t_ast));
    if (node != NULL) {
        node->type = type;
        node->command = command;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void free_args(t_command *command)
{
	int i = -1;
	while(++i < command->arg_count)
		free(command->args[i]);
	free(command->args);
}

void free_redirections(t_redirection *redirections)
{
	if(redirections == NULL)
		return ;
	free_redirections(redirections->next);
	free(redirections->file);
}

void free_command(t_command *command)
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
		command->args = malloc(sizeof(char *) * 2);
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
		printf("here ---------\n");
		print_token((*curr_token));
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
		{
			add_node_arg(command, (*curr_token)->value);
		} else
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
   t_ast* lhs_ast = extract_command(curr_token);
	if(lhs_ast == NULL)
		return NULL;
	if((*curr_token)->type == TOKEN_CP && is_in_op)
		return lhs_ast;
	if((*curr_token)->type != TOKEN_EOF && !token_is_operator(*curr_token))
	{
		if((*curr_token)->type == TOKEN_WORD)
			printf("minishell: parse error near '%s'\n", (*curr_token)->value);
		else 
			print_parse_error_near((*curr_token));
		// print_token((*curr_token));
		free_ast(lhs_ast);
		return NULL;
	}
    while ((*curr_token)->type != TOKEN_EOF && token_is_operator(*curr_token) && token_precedence(*curr_token) >= min_precedence) {
		t_token* op_token = *curr_token;
        *curr_token = ++(*curr_token); // Move past the operator.

        t_ast* rhs_ast = parse_expression(curr_token, token_precedence(op_token) + 1, is_in_op);
		if(rhs_ast == NULL)
		{
			free_ast(lhs_ast);
			return NULL;
		}
		
        t_ast* new_ast = create_ast_node(determine_node_type(op_token), NULL); // Convert token to a node type.
        new_ast->left = lhs_ast; // Attach the LHS AST as the left child.
        new_ast->right = rhs_ast; // Attach the RHS AST as the right child.

        lhs_ast = new_ast; // The new AST becomes the LHS of the next iteration (if any).
    }
    return lhs_ast;
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
        printf("-Empty Command\n");
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



int main() {
	char *str = "ls  ls  | grep thing > test < thing << a >> b || ls || <  ygerfuy | test && ((ls >> test | ls | test && node >> n)) || test \" ()  test &&  pipe | < > << >> || \"";
	// char *str = "| ls > ls > ls | ls";
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
