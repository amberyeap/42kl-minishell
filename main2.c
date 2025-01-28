/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:47:50 by ayeap             #+#    #+#             */
/*   Updated: 2025/01/28 14:55:33 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define DELIMITERS " \t\n"

typedef enum e_token_type
{
	TOKEN_WORD, // commands like 'echo', 'ls', 'cat', etc.
	TOKEN_PIPE, // '|'
	TOKEN_REDIR_IN, // '<'
	TOKEN_HEREDOC, // '<<'
	TOKEN_REDIR_OUT, // '>'
	TOKEN_APPEND, // '>>'
	TOKEN_ERROR,
	TOKEN_EOF,
}	t_token_type;

typedef struct s_lexer
{
	char *value;
	t_token_type type;
	struct s_lexer *next;
	struct s_lexer *prev;
}	t_lexer;

// typedef struct s_command
// {
// 	char **args;
// 	char *input;
// 	char *output;
// 	struct s_command *next;
// }	t_command;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_HEREDOC,
	NODE_REDIR_OUT,
	NODE_APPEND,
}	t_node_type;

typedef struct s_node
{
	t_node_type type;
	char **args;
	char *heredoc_delim;
	char *in_file;
	char *out_file;
	struct s_node *left_node;
	struct s_node *right_node;
}	t_node;

typedef struct s_parser
{
	t_node *root;
	t_lexer *current_token;
	// t_lexer *lexer_list;
}	t_parser;

typedef struct s_minishell
{
	char *user_input;
	char **env;
	t_lexer *lexer_list;
	t_parser *parser_tree;
}	t_minishell;

/* ************************************************************************** */
/*                                                                            */
/*                                        utils.c                             */
/*                                                                            */
/* ************************************************************************** */

int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int ft_error(char *message)
{
	printf("Error: %s\n", message);
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                parser_utils.c                              */
/*                                                                            */
/* ************************************************************************** */

void	init_parser(t_minishell *shell)
{
	shell->parser_tree = malloc(sizeof(t_parser));
	if (!shell->parser_tree)
		ft_error("Memory allocation failed");
	shell->parser_tree->root = NULL;
	shell->parser_tree->current_token = shell->lexer_list;
	// shell->parser_tree->lexer_list = shell->lexer_list;
}

// "consumes" the current token and moves on to the next one
void consume_token(t_parser *parser)
{
	if (parser->current_token->next)
		parser->current_token = parser->current_token->next;
	// else
	// 	parser->current_token->type = TOKEN_EOF;
}

t_node *create_node(t_node_type type)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
		ft_error("Memory allocation failed");
	node->type = type;
	node->args = NULL;
	node->heredoc_delim = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->left_node = NULL;
	node->right_node = NULL;
	return (node);
}

int match_token(t_parser *parser, t_toke_type type)
{
	return (parser->current_token->type == type);
}

/* ************************************************************************** */
/*                                                                            */
/*                                      parser.c                              */
/*                                                                            */
/* ************************************************************************** */

// parses redirections like '<', '<<', '>', '>>'
t_node *parse_redirections(t_parser *parser)
{}

t_node *parse_command(t_parser *parser)
{}

t_node *parse_command_suffix(t_parser *parser)
{

}

t_node *parse_command_prefix(t_parser *parser)
{

}

// parses simple commands like 'echo', 'ls', 'cat', etc.
t_node *parse_simple_command(t_parser *parser)
{
	t_node *prefix;
	t_node *suffix;
	t_node *command;

	prefix = parse_command_prefix(parser);
	suffix = parse_command_suffix(parser);
	command = parse_command(parser);
}

// parses pipes '|'
t_node *parse_pipe(t_parser *parser)
{
	t_node *left_node;
	t_node *pipe_node;

	left_node = parse_simple_command(parser);
	while (match_token(parser, TOKEN_PIPE))
	{
		pipe_node = create_node(NODE_PIPE);
		consume_token(parser);
		pipe_node->left_node = left_node;
		pipe_node->right_node = parse_simple_command(parser);
		left_node = pipe_node;
	}
	return (left_node);
}

t_node	*parse(t_minishell *shell)
{
	init_parser(shell);
	shell->parser_tree->root = parse_pipe(shell->parser_tree);
}

/* ************************************************************************** */
/*                                                                            */
/*                                     signals.c                              */
/*                                                                            */
/* ************************************************************************** */

// handles the Ctrl+C button in interactive mode - SIGINT signal
void sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handle_signals(void)
{
	signal(SIGINT, sigint_handler);
}

/* ************************************************************************** */
/*                                                                            */
/*                                 lexer_utils.c                              */
/*                                                                            */
/* ************************************************************************** */

// TAKE NOTE - may not work as intended
int count_quotes(char *line)
{
	int single_quotes;
	int double_quotes;

	single_quotes = 0;
	double_quotes = 0;

	// count the number of single and double quotes
	while (*line)
	{
		if (*line == 34)
			double_quotes++;
		else if (*line == 39)
			single_quotes++;
		line++;
	}
	// if there are odd number of quotes, return 0
	if ((single_quotes > 0 && single_quotes % 2 != 0) || (double_quotes > 0 && double_quotes % 2 != 0))
		return (0);
	return (1);
}

// not necessary
// doesn't work properly - doesnt break out of loop until two closed quotes are found in one line
void handle_missing_closed_quotes(t_minishell *shell)
{
	char *closed_quote;
	char *new_input;
	int len;

	while (1)
	{
		closed_quote = readline("> ");
		if (!closed_quote)
			break ;
		len = ft_strlen(shell->user_input) + ft_strlen(closed_quote) + 1;
		new_input = malloc(len + 1);
		if (!new_input)
			ft_error("Memory allocation failed");
		ft_strlcpy(new_input, shell->user_input, len);
		ft_strlcat(new_input, "\n", len);
		ft_strlcat(new_input, closed_quote, len);
		free(shell->user_input);
		shell->user_input = new_input;
		if (count_quotes(shell->user_input))
			break ;
	}
}

t_lexer init_lexer_node(char *value, t_token_type type)
{
	t_lexer lexer;

	lexer.value = value;
	lexer.type = type;
	lexer.next = NULL;
	lexer.prev = NULL;
	return (lexer);
}

void add_lexer(t_lexer *lexer_list, char *value, t_token_type type)
{
	t_lexer *current;
	t_lexer new_node;

	new_node = init_lexer_node(value, type);
	current = lexer_list;
	while (current->next)
		current = current->next;
	current->next = malloc(sizeof(t_lexer));
	if (!current->next)
		ft_error("Memory allocation failed");
	*current->next = new_node;
	current->next->prev = current;
}

int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/* ************************************************************************** */
/*                                                                            */
/*                                        lexer.c                             */
/*                                                                            */
/* ************************************************************************** */

int check_token(char *str, int i)
{
	if (str[i] == '|')
		return (TOKEN_PIPE);
	else if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (TOKEN_HEREDOC); // need to i++ to move the pointer?
		return (TOKEN_REDIR_IN);
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (TOKEN_APPEND); // need to i++?
		return (TOKEN_REDIR_OUT);
	}
	return (0);
}

int	handle_token(t_minishell *shell, int i)
{
	int token_type;

	token_type = check_token(shell->user_input, i);
	if (token_type == TOKEN_PIPE)
		add_lexer(shell->lexer_list, NULL, TOKEN_PIPE);
	else if (token_type == TOKEN_REDIR_IN)
		add_lexer(shell->lexer_list, NULL, TOKEN_REDIR_IN);
	else if (token_type == TOKEN_HEREDOC)
	{
		add_lexer(shell->lexer_list, NULL, TOKEN_HEREDOC);
		return (2);
	}
	else if (token_type == TOKEN_REDIR_OUT)
		add_lexer(shell->lexer_list, NULL, TOKEN_REDIR_OUT);
	else if (token_type == TOKEN_APPEND)
	{
		add_lexer(shell->lexer_list, NULL, TOKEN_APPEND);
		return (2);
	}
	return (1);
}

int is_quote(char c)
{
	return (c == 34 || c == 39);
}

int handle_quotes(char *str, int i)
{
	int j;

	j = 1;
	while (str[i + j] && str[i + j] != str[i])
		j++;
	if (!str[i + j])
		return (-1);
	return (j);
}

int handle_non_tokens(t_lexer *lexer_list, char *str, int i)
{
	int j;

	j = 0;
	while (str[i + j] && !check_token(str, i + j))
	{
		if (is_quote(str[i + j]))
			j += handle_quotes(str, i + j);
		if (is_whitespace(str[i + j]))
			break ;
		else
			j++;
	}
	if (j > 0)
		add_lexer(lexer_list, ft_substr(str, i, j), TOKEN_WORD);
	return (j);
}

int 	lexer(t_minishell *shell)
{
	int i;
	int j;

	i = 0;
	while(shell->user_input[i])
	{
		j = 0;
		if (is_whitespace(shell->user_input[i]))
			i++;
		if (check_token(shell->user_input, i))
			j += handle_token(shell, i);
		else
			j += handle_non_tokens(shell->lexer_list, shell->user_input, i);
		if (j < 0)
			return (0);
		i += j;
	}
	return (1);
}

void test_lexer(t_minishell *shell)
{
	t_lexer *current;

	current = shell->lexer_list;
	while (current)
	{
		printf("Value: %s\n", current->value);
		printf("Type: %d\n", current->type);
		current = current->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                        main.c                              */
/*                                                                            */
/* ************************************************************************** */

int minishell_loop(t_minishell *shell)
{
	handle_signals();
	while (1)
	{
		shell->user_input = readline("minishell$ ");
		if (!shell->user_input)
			break;
		// add_history(line);
		if (!count_quotes(shell->user_input))
			ft_error("Missing closed quotes");
			// handle_missing_closed_quotes(shell);
		if (!lexer(shell))
			ft_error("Lexer failed");
		test_lexer(shell);
		free(shell->user_input);
	}
	return (0);
}

void init_shell(t_minishell *shell, char **env)
{
	shell->user_input = NULL;
	shell->env = env;
	shell->lexer_list = malloc(sizeof(t_lexer));
	if (!shell->lexer_list)
		ft_error("Memory allocation failed");
}

int main(int argc, char **argv, char **env)
{
	t_minishell shell;

	(void)argv;
	if (argc > 1)
		return (printf("This program doesn't take any arguments.\n"), 1);
	init_shell(&shell, env);
	minishell_loop(&shell);
	shell->parser_tree = parse(&shell);
	return (0);
}
