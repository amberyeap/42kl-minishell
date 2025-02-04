#include "minishell.h"

#define WHTIESPACE " \t\n"
#define METACHARS "|&;<>() \t\n"
#define OPERATORS "|<>"

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

typedef enum s_ast_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_HEREDOC,
	NODE_APPEND
}	t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			**args;
	char			*heredoc_delim;
	char			*redir_file;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_parser
{
	t_token		*current_token;
	t_token		*token_list;
	int			error;
	char		*error_msg;
}	t_parser;

typedef struct s_minishell
{
	char		*user_input;
	char		**env;
	t_token		*token_list;
	t_ast	*parser_tree;
}	t_minishell;

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
/*                         parser_create_nodes.c                              */
/*                                                                            */
/* ************************************************************************** */



/* ************************************************************************** */
/*                                                                            */
/*                                parser_utils.c                              */
/*                                                                            */
/* ************************************************************************** */

// "consumes" the current token by moving to the next token
void	consume_token(t_parser *parser)
{
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
}

// matches the current token with the expected type
int	match_token(t_parser *parser, t_token_type type)
{
	return (parser->current_token && parser->current_token->type == type);
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

int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

void	set_parser_error(t_parser *parser, char *msg)
{
	parser->error = 1;
	parser->error_msg = ft_strdup(msg);
}

/* ************************************************************************** */
/*                                                                            */
/*                                      parser.c                              */
/*                                                                            */
/* ************************************************************************** */

t_ast	*create_ast_node(t_ast_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->heredoc_delim = NULL;
	node->redir_file = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_ast_node(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->args)
	{
		i = 0;
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	if (node->heredoc_delim)
		free(node->heredoc_delim);
	if (node->redir_file)
		free(node->redir_file);
	free_ast_node(node->left);
	free_ast_node(node->right);
	free(node);
}

t_ast	*check_node_type(t_token_type type)
{
	t_ast	*node;

	node = NULL;
	if (type == TOKEN_PIPE)
		node = create_ast_node(NODE_PIPE);
	else if (type == TOKEN_REDIR_IN)
		node = create_ast_node(NODE_REDIR_IN);
	else if (type == TOKEN_REDIR_OUT)
		node = create_ast_node(NODE_REDIR_OUT);
	else if (type == TOKEN_HEREDOC)
		node = create_ast_node(NODE_HEREDOC);
	else if (type == TOKEN_APPEND)
		node = create_ast_node(NODE_APPEND);
	return (node);
}

t_ast	*parse_redirection(t_parser *parser, t_ast *cmd_node)
{
	t_ast			*redir_node;
	t_token_type	redir_type;

	// printf("parsing redirection\n");
	redir_type = parser->current_token->type;
	consume_token(parser);
	if (!match_token(parser, TOKEN_WORD))
	{
		set_parser_error(parser, "Expected filename after redirection");
		return (NULL);
	}
	redir_node = check_node_type(redir_type);
	// printf("node checked\n");
	if (!redir_node)
		return (NULL);
	if (redir_type == TOKEN_HEREDOC)
		redir_node->heredoc_delim = ft_strdup(parser->current_token->value);
	else
		redir_node->redir_file = ft_strdup(parser->current_token->value);
	redir_node->left = cmd_node;
	consume_token(parser);
	return (redir_node);
}

t_ast	*parse_simple_command(t_parser *parser)
{
	t_ast	*node;
	t_token	*start_token;
	int		arg_count;
	int		i;

	// printf("parsing simple command\n");
	node = NULL;
	start_token = parser->current_token;
	arg_count = 0;
	i = 0;
	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		arg_count++;
		consume_token(parser);
	}
	// printf("args counted\n");
	if (arg_count == 0)
		return (NULL);
	node = create_ast_node(NODE_COMMAND);
	// printf("node created\n");
	if (!node)
		return (NULL);
	node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!node->args)
		return (free(node), NULL);
	parser->current_token = start_token;
	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		node->args[i] = ft_strdup(parser->current_token->value);
		consume_token(parser);
		i++;
	}
	// printf("args copied\n");
	node->args[i] = NULL;
	while (parser->current_token && is_redir_token(parser->current_token->type))
	{
		// printf("found redir\n");
		node = parse_redirection(parser, node);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;
	t_ast	*pipe_node;

	// printf("parsing pipeline\n");

	left = parse_simple_command(parser);
	if (!left)
		return (NULL);
	while (match_token(parser, TOKEN_PIPE))
	{
		pipe_node = create_ast_node(NODE_PIPE);
		if (!pipe_node)
			return (NULL);
		pipe_node->left = left;
		consume_token(parser);
		pipe_node->right = parse_simple_command(parser);
		if (!pipe_node->right)
		{
			set_parser_error(parser, "Expected command after pipe");
			free_ast_node(pipe_node);
			return (NULL);
		}
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse(t_token *token_list)
{
	t_parser	*parser;
	t_ast		*root;

	if (!token_list)
		return (NULL);
	parser = init_parser(token_list);
	// printf("parser initialized\n");
	root = parse_pipeline(parser);
	if (parser->error)
	{
		if (root)
			free_ast_node(root);
		root = NULL;
	}
	if (parser->error_msg)
		free(parser->error_msg);
	free(parser);
	return (root);
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
	token->value = value;
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

// creates a word token
t_token *create_word_token(t_tokenizer *tokenizer)
{
	int		start;
	int		len;
	char	*word;

	start = tokenizer->position;
	while (!is_whitespace(peek(tokenizer)) && !is_operator(peek(tokenizer)) && peek(tokenizer))
		next_char(tokenizer);
	len = tokenizer->position - start;
	word = ft_substr(tokenizer->user_input, start, len);
	if (!word)
	{
		tokenizer->error = 1;
		tokenizer->error_msg = ft_strdup("Memory allocation failed");
		return (NULL);
	}
	return (create_token(TOKEN_WORD, word));
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
	return (NULL);
}

// main tokenize function
/* 1. skips whitespace
** 2. checks the char in the user_input.
**	- if it's a nul terminator, it will break out of the loop and create a EOF token
**	- if it's an operator, it will create an operator token
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
	if (!tokenizer->error)
	{
		new_token = create_token(TOKEN_EOF, NULL);
		add_token(&head, new_token);
	}
	if (tokenizer->error)
	{
		if (tokenizer->error_msg)
			free(tokenizer->error_msg);
		free_tokens(head);
		head = NULL;
	}
	free(tokenizer);
	return (head);
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

void print_ast(t_ast *node, int depth)
{
    int i;

    if (!node)
        return;

    // Print indentation
    for (i = 0; i < depth; i++)
        printf("  ");

    // Print node type
    switch (node->type)
    {
        case NODE_COMMAND:
            printf("COMMAND: ");
            i = 0;
            while (node->args && node->args[i])
            {
                printf("%s ", node->args[i]);
                i++;
            }
            printf("\n");
            break;
        case NODE_PIPE:
            printf("PIPE\n");
            break;
        case NODE_REDIR_IN:
            printf("REDIR_IN: %s\n", node->redir_file);
            break;
        case NODE_REDIR_OUT:
            printf("REDIR_OUT: %s\n", node->redir_file);
            break;
        case NODE_HEREDOC:
            printf("HEREDOC: %s\n", node->heredoc_delim);
            break;
        case NODE_APPEND:
            printf("APPEND: %s\n", node->redir_file);
            break;
    }

    // Recursively print children
    if (node->left)
        print_ast(node->left, depth + 1);
    if (node->right)
        print_ast(node->right, depth + 1);
}

/* Test cases */
void test_parser(void)
{
    // Test case structure
    struct test_case {
        char *input;
        char *description;
    };

    // Define test cases
    struct test_case tests[] = {
        {"ls -l", "Simple command"},
        {"ls -l | grep foo", "Simple pipe"},
        {"ls -l|grep foo", "Pipe without spaces"},
        {"cat < input.txt", "Input redirection"},
        {"echo hello > output.txt", "Output redirection"},
        {"echo hello >> output.txt", "Append redirection"},
        {"cat << EOF", "Heredoc"},
        {"ls -l | grep foo > output.txt", "Pipe with redirection"},
        {"ls -l|grep foo>output.txt", "Pipe with redirection, no spaces"},
        {"cat < input.txt | grep foo > output.txt", "Complex command"},
        {"ls     -l     |    grep    foo", "Multiple spaces"},
        {"ls", "Single command"},
        {"", "Empty input"},
		{"ls -l | grep foo | wc -l", "Multiple pipes"},
		{"ls -l | grep foo | wc -l > output.txt", "Multiple pipes with redirection"},
        {NULL, NULL}
    };

    int i = 0;
    while (tests[i].input != NULL)
    {
        printf("\nTest %d: %s\n", i + 1, tests[i].description);
        printf("Input: '%s'\n", tests[i].input);
        printf("Output AST:\n");

        // Tokenize
        t_token *tokens = tokenize(tests[i].input);
        if (!tokens)
        {
            printf("Tokenization failed!\n");
            i++;
            continue;
        }

        // Print tokens for debugging
        printf("Tokens:\n");
        print_tokens(tokens);

        // Parse
        t_ast *ast = parse(tokens);
        if (!ast)
        {
            printf("Parsing failed!\n");
            free_tokens(tokens);
            i++;
            continue;
        }
		printf("\nPARSER\n");
        // Print AST
        print_ast(ast, 0);

        // Cleanup
        free_ast_node(ast);
        free_tokens(tokens);
        i++;
    }
}

int main(void)
{
    test_parser();
    return 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                        main.c                              */
/*                                                                            */
/* ************************************************************************** */

// void	ft_error(char *msg)
// {
// 	printf("%s\n", msg);
// 	exit(1);
// }

// void	init_shell(t_minishell *shell, char **env)
// {
// 	shell->user_input = NULL;
// 	shell->env = env;
// 	shell->token_list = NULL;
// 	shell->parser_tree = NULL;
// }

// int main(int argc, char **argv, char **envp)
// {
// 	char		*line;
// 	t_minishell	shell;

// 	(void)argv;
// 	if (argc != 1)
// 		return(printf("Program does not accept arguments.\n"), 1);
// 	init_shell(&shell, envp);
// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (!line)
// 			break ;
// 		shell.user_input = line;
// 		shell.token_list = tokenize(shell.user_input);
// 		printf("tokenized\n");
// 		if (shell.token_list)
// 		{
// 			printf("entering parse\n");
// 			shell.parser_tree = parse(shell.token_list);
// 			printf("parsed\n");
// 			print_tokens(shell.token_list);
// 			if (shell.parser_tree)
// 				print_ast(shell.parser_tree, 0);
// 			free_ast_node(shell.parser_tree);

// 		}
// 		free(shell.token_list);
// 		shell.token_list = NULL;
// 		free(shell.user_input);
// 		shell.user_input = NULL;
// 	}
// }