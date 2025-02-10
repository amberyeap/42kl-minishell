#include "minishell.h"

// current checklist for parser:
// - syntax error
//   - tokens DONE
//   - parser DONE-ish -> calls error but not the same error message as actual bash
// - handle quotes DONE-ish -> need to handle words that are joined to the quoted word (eg: "hello"world)
// - empty input DONE
// - functions too long

// - handle expansion -> done in execution section
// - handle exit codes 
// - handle signals
// - handle history

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_ERROR,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_tokenizer 
{
	char	*user_input;
	int		position;
	int		error;
	char	*error_msg;
}	t_tokenizer;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_ARG,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_HEREDOC,
	NODE_APPEND,
	NODE_EOF
}	t_node_type;

typedef enum e_direction
{
	INPUT,
	OUTPUT
}	t_direction;

typedef struct s_parser
{
	t_token		*current_token;
	t_token		*token_list;
	int			error;
	char		*error_msg;
}	t_parser;

typedef struct s_node
{
	t_node_type		type;
	char			*cmd;
	// char			*arg;
	char			**arg; // if NODE_ARG is not used, then this is not needed
	char			*heredoc_delim;
	char			*redir_file;
	t_direction		redir_direction; // the direction of the redir token
	struct s_node	*next;
}	t_node;

typedef struct s_minishell
{
	char		*user_input;
	char		**env;
	t_token		*token_list; // pointer to the head of the token list
	t_node		*parser_list; // pointer to the first node of the list
	t_parser	*parser; // pointer to the parser - used to pass to the parse function
}	t_minishell;

t_node	*parse_simple_command(t_parser *parser, t_node *head);

void print_tokens(t_token *head);
void print_nodes(t_node *head);

/* ************************************************************************** */
/*                                                                            */
/*                                       utils.c                              */
/*                                                                            */
/* ************************************************************************** */

// returns a number (aka true) if the char is a whitespace
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// returns a number (aka true) if the char is an operator
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* ************************************************************************** */
/*                                                                            */
/*                                parser_utils.c                              */
/*                                                                            */
/* ************************************************************************** */

int is_redir_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (1);
	return (0);
}

void	consume_token(t_parser *parser)
{
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
}

void	set_parser_error(t_parser *parser, char *msg)
{
	parser->error = 1;
	parser->error_msg = ft_strdup(msg);
}

void	add_node_to_back(t_node *head, t_node *new_node)
{
	t_node	*current;

	if (!head)
		return ;
	current = head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

// initializes the parser
t_parser	*init_parser(t_token *token_list)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->token_list = token_list;
	parser->current_token = token_list;
	parser->error = 0;
	parser->error_msg = NULL;
	return (parser);
}

void free_ast(t_node *head)
{
	t_node	*current;
	t_node	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->arg)
			free(current->arg);
		if (current->heredoc_delim)
			free(current->heredoc_delim);
		if (current->redir_file)
			free(current->redir_file);
		free(current);
		current = next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                      parser.c                              */
/*                                                                            */
/* ************************************************************************** */

t_node	*create_node(t_node_type type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = NULL;
	node->arg = NULL;
	node->heredoc_delim = NULL;
	node->redir_file = NULL;
	node->redir_direction = -1;
	node->next = NULL;
	return (node);
}

t_node	*create_redir_node(t_token_type type)
{
	t_node	*redir_node;

	redir_node = NULL;
	if (type == TOKEN_APPEND)
	{
		redir_node = create_node(NODE_APPEND);
		redir_node->redir_direction = OUTPUT;
	}
	else if (type == TOKEN_REDIR_IN)
	{
		redir_node = create_node(NODE_REDIR_IN);
		redir_node->redir_direction = INPUT;
	}
	else if (type == TOKEN_REDIR_OUT)
	{
		redir_node = create_node(NODE_REDIR_OUT);
		redir_node->redir_direction = OUTPUT;
	}
	else if (type == TOKEN_HEREDOC)
	{
		redir_node = create_node(NODE_HEREDOC);
		redir_node->redir_direction = INPUT;
	}
	return (redir_node);
}

void	parse_redirection(t_parser *parser, t_node *head)
{
	t_node			*redir_node;
	t_token_type	redir_type;

	redir_type = parser->current_token->type; // store the type of redirection
	redir_node = create_redir_node(redir_type);
	if (!redir_node)
	{
		set_parser_error(parser, "Failed to create redirection node");
		return ;
	}
	consume_token(parser); // move to the next token
	if (redir_type == TOKEN_HEREDOC && parser->current_token->type != TOKEN_WORD)
	{
		set_parser_error(parser, "Expected delimiter after heredoc redirection");
		return;
	}
	else if (parser->current_token->type != TOKEN_WORD)
	{
		set_parser_error(parser, "Expected filename after redirection");
		return;
	}
	if (redir_type == TOKEN_HEREDOC)
		redir_node->heredoc_delim = ft_strdup(parser->current_token->value);
	else
		redir_node->redir_file = ft_strdup(parser->current_token->value);
	consume_token(parser);
	add_node_to_back(head, redir_node);
}

void	parse_pipe(t_parser *parser, t_node *head)
{
	t_node	*pipe_node;

	pipe_node = create_node(NODE_PIPE);
	add_node_to_back(head, pipe_node);
	consume_token(parser);
	parse_simple_command(parser, pipe_node);
}

// this adds arguments to the command node as multiple strings
void	add_arg_to_command(char *arg, t_node *cmd_node)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd_node->arg && cmd_node->arg[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	i = 0;
	while (cmd_node->arg && cmd_node->arg[i])
	{
		new_args[i] = ft_strdup(cmd_node->arg[i]);
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd_node->arg);
	cmd_node->arg = new_args;
}

// void	parse_arguments(t_parser *parser, t_node *head)
// {
// 	t_node	*arg_node;

// 	arg_node = create_node(NODE_ARG);
// 	if (!arg_node)
// 	{
// 		set_parser_error(parser, "Failed to create argument node");
// 		return ;
// 	}
// 	arg_node->arg = ft_strdup(parser->current_token->value);
// 	add_node_to_back(head, arg_node);
// }

t_node	*create_command_node(char *cmd, t_node_type type)
{
	t_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->cmd = ft_strdup(cmd);
	return (node);
}

t_node	*parse_simple_command(t_parser *parser, t_node *head)
{
	t_node	*first_node;

	if (!parser->current_token)
		return (NULL);
	if (is_redir_token(parser->current_token->type))
		parse_redirection(parser, head);
	else if (parser->current_token->type == TOKEN_WORD)
	{
		first_node = create_command_node(parser->current_token->value, NODE_COMMAND);
		add_node_to_back(head, first_node);
		consume_token(parser);
		while (parser->current_token && parser->current_token->type == TOKEN_WORD)
		{
			add_arg_to_command(parser->current_token->value, first_node);
			// parse_arguments(parser, head);
			consume_token(parser);
		}
		if (parser->current_token->type == TOKEN_PIPE)
			parse_pipe(parser, head);
		if (is_redir_token(parser->current_token->type))
			parse_redirection(parser, head);
	}
	return (first_node);
}

t_node	*parse(t_parser *parser)
{
	t_node *head;

	head = NULL;
	if (!parser->token_list)
		return (NULL);
	head = parse_simple_command(parser, head);
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                tokens_utils.c                              */
/*                                                                            */
/* ************************************************************************** */

// get current character in the user_input
char	peek(t_tokenizer *tokenizer)
{
	return (tokenizer->user_input[tokenizer->position]);
}

// moves to the next character in the user_input
void	next_char(t_tokenizer *tokenizer)
{
	tokenizer->position++;
}

// skips whitespace
void	skip_whitespace(t_tokenizer *tokenizer)
{
	while (peek(tokenizer) && is_whitespace(peek(tokenizer)))
		next_char(tokenizer);
}

// initializes the tokenizer
t_tokenizer	*init_tokenizer(char *input)
{
	t_tokenizer	*tokenizer;

	tokenizer = malloc(sizeof(t_tokenizer));
	if (!tokenizer)
		return (NULL);
	tokenizer->user_input = input;
	tokenizer->position = 0;
	tokenizer->error = 0;
	tokenizer->error_msg = NULL;
	return (tokenizer);
}

void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                      tokens.c                              */
/*                                                                            */
/* ************************************************************************** */

// creates a new token
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

// adds a token to the end of the list
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*handle_quotes(t_tokenizer *tokenizer)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	quote = peek(tokenizer);
	next_char(tokenizer);
	start = tokenizer->position;
	while (peek(tokenizer) != quote && peek(tokenizer))
		next_char(tokenizer);
	len = tokenizer->position - start;
	if (peek(tokenizer) != quote)
		return (NULL);
	word = ft_substr(tokenizer->user_input, start, len);
	next_char(tokenizer);
	return (word);
}

// creates a word token
/* 1. if the current char is a quote, it will call handle_quotes() (34 is double quote, 39 is single quote)
*/
t_token *create_word_token(t_tokenizer *tokenizer)
{
	int		start;
	int		len;
	char	*word;
	t_token	*token;

	if (peek(tokenizer) == 34 || peek(tokenizer) == 39)
	{
		word = handle_quotes(tokenizer);
		if (!word)
			return (create_token(TOKEN_ERROR, "Unterminated quote"));
	}
	else
	{
		start = tokenizer->position;
		while (!is_whitespace(peek(tokenizer)) && !is_operator(peek(tokenizer)) && peek(tokenizer))
			next_char(tokenizer);
		len = tokenizer->position - start;
		word = ft_substr(tokenizer->user_input, start, len);
	}
	if (word)
	{
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
	}
	return (create_token(TOKEN_ERROR, "Failed to create word token")); // most likely will not reach here?
}

// creates an operator token
t_token	*create_op_token(t_tokenizer *tokenizer)
{
	if (peek(tokenizer) == '|')
	{
		next_char(tokenizer);
		return (create_token(TOKEN_PIPE, NULL));
	}
	else if (peek(tokenizer) == '<')
	{
		next_char(tokenizer);
		if (peek(tokenizer) == '<')
		{
			next_char(tokenizer);
			return (create_token(TOKEN_HEREDOC, NULL));
		}
		else
			return (create_token(TOKEN_REDIR_IN, NULL));
	}
	else if (peek(tokenizer) == '>')
	{
		next_char(tokenizer);
		if (peek(tokenizer) == '>')
		{
			next_char(tokenizer);
			return (create_token(TOKEN_APPEND, NULL));
		}
		else
			return (create_token(TOKEN_REDIR_OUT, NULL));
	}
	return (create_token(TOKEN_ERROR, "Unrecognized operator")); // most likely will not reach here?
}

// main tokenize function
/* 1. skips whitespace
** 2. checks the char in the user_input.
**	- if it's a nul terminator, it will break out of the loop and create a EOF token
**	- if it's an operator (|<>), it will create an operator token
**	- else if it's a word, it will create a word token 
** 3. check if there was any error encountered
**	- if NO error, create EOF token
**	- if there is error, free all malloc and set the head pointer to NULL 
** 4. free tokenizer as the user_input has been tokenized
** 5. return the head pointer, which is a pointer to a list of tokens
*/
t_token	*tokenize(char *input)
{
	t_tokenizer	*tokenizer;
	t_token		*head;
	t_token		*new_token;

	head = NULL;
	tokenizer = init_tokenizer(input);
	while (peek(tokenizer))
	{
		skip_whitespace(tokenizer);
		if (peek(tokenizer) == '\0')
			break ;
		if (is_operator(peek(tokenizer)))
			new_token = create_op_token(tokenizer);
		else
			new_token = create_word_token(tokenizer);
		add_token(&head, new_token);
	}
	// if (!tokenizer->error)
	// {
	// 	new_token = create_token(TOKEN_EOF, NULL);
	// 	add_token(&head, new_token);
	// }
	// if (tokenizer->error)
	// {
	// 	if (tokenizer->error_msg)
	// 		free(tokenizer->error_msg);
	// 	free_tokens(head);
	// 	head = NULL;
	// }
	free(tokenizer);
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                        main.c                              */
/*                                                                            */
/* ************************************************************************** */

void	ft_error(t_minishell *shell, char *msg)
{
	printf("%s\n", msg);
	free_tokens(shell->token_list);
}

void	init_shell(t_minishell *shell, char **env)
{
	shell->user_input = NULL;
	shell->env = env;
	shell->token_list = NULL;
	shell->parser_list = NULL;
	shell->parser = NULL;
}

void reset_main(t_minishell *shell)
{
	free_tokens(shell->token_list);
	// free_ast_node(shell->parser_tree);
	free(shell->user_input);
	shell->token_list = NULL;
	shell->parser_list = NULL;
	shell->user_input = NULL;
}

int	check_token_error(t_minishell *shell)
{
	t_token	*current;

	current = shell->token_list;
	while (current)
	{
		if (current->type == TOKEN_ERROR)
		{
			printf("%s\n", current->value);
			free_tokens(shell->token_list);
			shell->token_list = NULL;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	main_parse(t_minishell *shell)
{
	shell->parser = init_parser(shell->token_list);
	if (!shell->parser)
	{
		fprintf(stderr, "Failed to initialize parser\n");
		free_tokens(shell->token_list);
		free(shell->user_input);
		shell->token_list = NULL;
		shell->user_input = NULL;
	}
	shell->parser_list = parse(shell->parser);
	if (!shell->parser_list)
	{
		fprintf(stderr, "%s\n", shell->parser->error_msg);
		free_tokens(shell->token_list);
		free(shell->parser->error_msg);
		free(shell->user_input);
		shell->token_list = NULL;
		shell->user_input = NULL;
	}
}

void cleanup(t_minishell *shell)
{
	// free_ast_node(shell->parser_list);
	free_tokens(shell->token_list);
	free(shell->user_input);
	shell->parser_list = NULL;
	shell->token_list = NULL;
	shell->user_input = NULL;
}

int main(int argc, char **argv, char **envp)
{
	char		*line;
	t_minishell	shell;

	(void)argv;
	if (argc != 1)
		return(printf("Program does not accept arguments.\n"), 1);
	init_shell(&shell, envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (!*line)
		{
			free(line);
			continue ;
		}
		// add_history(line);
		shell.user_input = line;
		shell.token_list = tokenize(shell.user_input);
		if (check_token_error(&shell))
		{
			free(shell.user_input);
			shell.user_input = NULL;
			continue ;
		}
		else
			main_parse(&shell);
		print_tokens(shell.token_list);
		print_nodes(shell.parser_list);
		cleanup(&shell);
	}
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                        test.c                              */
/*                                                                            */
/* ************************************************************************** */

/* Debug function to print tokens */
void print_tokens(t_token *head)
{
    t_token *current = head;
    const char *type_strings[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
        "HEREDOC", "APPEND", "ERROR", "EOF"
    };

    while (current) {
        printf("Type: %-10s", type_strings[current->type]);
        if (current->value)
            printf(" Value: '%s'", current->value);
        printf("\n");
        current = current->next;
    }
}

void print_nodes(t_node *head)
{
	t_node	*current;

	current = head;
	// while (current)
	// {
	// 	if (current->type == NODE_COMMAND)
	// 		printf("Command: %s\n", current->cmd);
	// 	// else if (current->type == NODE_ARG)
	// 	// 	printf("Arg: %s\n", current->arg);
	// 	else if (current->type == NODE_PIPE)
	// 		printf("Pipe\n");
	// 	else if (current->type == NODE_REDIR_IN)
	// 		printf("Redir in: %s\n", current->redir_file);
	// 	else if (current->type == NODE_REDIR_OUT)
	// 		printf("Redir out: %s\n", current->redir_file);
	// 	else if (current->type == NODE_HEREDOC)
	// 		printf("Heredoc: %s\n", current->heredoc_delim);
	// 	else if (current->type == NODE_APPEND)
	// 		printf("Append: %s\n", current->redir_file);
	// 	current = current->next;
	// }
	while (current)
	{
		if (current->type == NODE_COMMAND)
		{
			printf("Command: %s\n", current->cmd);
			if (current->arg)
			{
				for (int i = 0; current->arg[i]; i++)
					printf("Arg: %s\n", current->arg[i]);
			}
		}
		else if (current->type == NODE_PIPE)
			printf("Pipe\n");
		else if (current->type == NODE_REDIR_IN)
			printf("Redir in: %s\n", current->redir_file);
		else if (current->type == NODE_REDIR_OUT)
			printf("Redir out: %s\n", current->redir_file);
		else if (current->type == NODE_HEREDOC)
			printf("Heredoc: %s\n", current->heredoc_delim);
		else if (current->type == NODE_APPEND)
			printf("Append: %s\n", current->redir_file);
		current = current->next;
	}
}

/* Test cases */
// void test_parser(void)
// {
//     // Test case structure
//     struct test_case {
//         char *input;
//         char *description;
//     };

//     // Define test cases
//     struct test_case tests[] = {
//         {"ls -l", "Simple command"},
//         {"ls -l | grep foo", "Simple pipe"},
//         {"ls -l|grep foo", "Pipe without spaces"},
//         {"cat < input.txt", "Input redirection"},
//         {"echo hello > output.txt", "Output redirection"},
//         {"echo hello >> output.txt", "Append redirection"},
//         {"cat << EOF", "Heredoc"},
//         {"ls -l | grep foo > output.txt", "Pipe with redirection"},
//         {"ls -l|grep foo>output.txt", "Pipe with redirection, no spaces"},
//         {"cat < input.txt | grep foo > output.txt", "Complex command"},
//         {"ls     -l     |    grep    foo", "Multiple spaces"},
//         {"ls", "Single command"},
//         {"", "Empty input"},
// 		{"ls -l | grep foo | wc -l", "Multiple pipes"},
// 		{"ls -l | grep foo | wc -l > output.txt", "Multiple pipes with redirection"},
//         {NULL, NULL}
//     };

//     int i = 0;
//     while (tests[i].input != NULL)
//     {
//         printf("\nTest %d: %s\n", i + 1, tests[i].description);
//         printf("Input: '%s'\n", tests[i].input);
//         printf("Output AST:\n");

//         // Tokenize
//         t_token *tokens = tokenize(tests[i].input);
//         if (!tokens)
//         {
//             printf("Tokenization failed!\n");
//             i++;
//             continue;
//         }

//         // Print tokens for debugging
//         printf("Tokens:\n");
//         print_tokens(tokens);

//         // Parse
//         t_ast *ast = parse(tokens);
//         if (!ast)
//         {
//             printf("Parsing failed!\n");
//             free_tokens(tokens);
//             i++;
//             continue;
//         }
// 		printf("\nPARSER\n");
//         // Print AST
//         print_ast(ast, 0);

//         // Cleanup
//         free_ast_node(ast);
//         free_tokens(tokens);
//         i++;
//     }
// }

// int main(void)
// {
//     test_parser();
//     return 0;
// }
