# include <readline/readline.h> // readline, add_history, rl_clear_history,
							// rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>
# include <stdio.h> // printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // write, access, open, read, close, fork, pipe, isatty,
				// ttyname, ttyslot, getcwd, chdir, unlink, execve, dup, dup2
# include <fcntl.h> // open
# include <signal.h> // signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/types.h> // pid_t
# include <sys/stat.h> // stat, lstat, fstat
# include <dirent.h> // opendir, readdir, closedir
# include <errno.h> // errno
# include <string.h> // strerror
# include <termios.h> // tcsetattr, tcgetattr, ioctl
# include <curses.h> // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/wait.h> // wait, waitpid, wait3, wait4
# include <ctype.h>
# include <limits.h>

# include "../../library/inc/libft.h"

typedef enum e_token_type {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

// typedef enum e_quote_type {
// 	UNQUOTED,
// 	SINGLE_QUOTE,
// 	DOUBLE_QUOTE,
// 	UNTERMINATED
// }	t_quote_type;

typedef struct s_token {
	char			*value;
	t_token_type	type;
	// t_quote_type	quote;
	int				expand;
	struct s_token	*next;
}	t_token;

typedef struct s_tokenizer
{
	char	*user_input;
	int		position;
}	t_tokenizer;

typedef enum e_node_type {
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_HEREDOC,
	NODE_APPEND,
	NODE_EOF // might not be used
}	t_node_type;

typedef enum e_direction {
	INPUT,
	OUTPUT
}	t_direction;

// typedef struct s_parser_node {
// 	t_node_type				type;
// 	char					*cmd;
// 	char					**args;
// 	char					*heredoc_delim;
// 	char					*redir_file;
// 	t_direction				redir_direction;
// 	int						error;
// 	char					*error_msg;
// 	struct s_parser_node	*next;
// }	t_parser;

typedef struct s_parser
{
	t_token		*current_token;
	t_token		*token_list;
	int			error;
	char		*error_msg;
	char		**env;
}	t_parser;

typedef struct s_node
{
	t_node_type		type;
	char			*cmd;
	char			**arg;
	char			*heredoc_delim;
	char			*redir_file;
	t_direction		redir_direction;
	struct s_node	*next;
}	t_node;


typedef struct s_env
{
	char			*value;
	char			*key;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell {
	char		*user_input;

	char		**sorted_env;
	int			exit;
	int			ret;
	t_env		*env;

	t_token		*token_list;
	t_parser	*parser;
	t_node		*parser_list;
}	t_minishell;

void	parse_pipe(t_parser *parser, t_node **head);

/* ************************************************************************** */
/*                                                                            */
/*                                      test.c                               */
/*                                                                            */
/* ************************************************************************** */

void print_tokens(t_token *head)
{
    t_token *current = head;
    const char *type_strings[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
        "HEREDOC", "APPEND", "ERROR", "EOF"
    };
	const char *quote_strings[] = {
		"NO_QUOTE", "DOUBLE_QUOTE", "SINGLE_QUOTE", "UNTERMINATED QUOTE"
	};

    while (current) {
        printf("Type: %-10s", type_strings[current->type]);
        if (current->value)
            printf(" Value: '%s'", current->value);
		// if (current->quote)
		// 	printf(" Quote: %s", quote_strings[current->quote]);
		if (current->expand)
			printf(" Expand: %d", current->expand);
        printf("\n");
        current = current->next;
    }
}

void print_nodes(t_node *head)
{
	t_node	*current;

	current = head;
	while (current)
	{
		if (current->type == NODE_CMD)
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

/* ************************************************************************** */
/*                                                                            */
/*                                 expansion.c                                */
/*                                                                            */
/* ************************************************************************** */

char	*get_env_var(char **value, char *var_start, char *result, int *i)
{
	char	*expanded_var;
	char	*var_name;

	(*value)++;
	while (ft_isalnum(**value))
		(*value)++;
	var_name = ft_substr(*value, 0, *value - var_start);
	expanded_var = getenv(var_name);
	if (expanded_var)
		*i += ft_strlcpy(result + (*i), expanded_var,
				ft_strlen(expanded_var) + 1);
	free(var_name);
	return (result);
}

char	*expand_var(char *value)
{
	char	*var_start;
	char	*result;
	int		i;

	i = 0;
	result = malloc(1024);
	if (!result)
		return (NULL);
	var_start = ft_strchr(value, '$');
	if (!var_start)
		return (free(result), ft_strdup(value));
	while (*value)
	{
		if (*value == '$' && ft_isalnum(*(value + 1)) && *(value + 1))
		{
			if (!get_env_var(&value, var_start, result, &i))
				return (free(result), NULL);
		}
		else
			result[i++] = *value++;
	}
	result[i] = '\0';
	return (result);
}

// /* ************************************************************************** */
// /*                                                                            */
// /*                              parser_utils.c                                */
// /*                                                                            */
// /* ************************************************************************** */

int	is_redir_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
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

void	add_node_to_back(t_node **head, t_node *new_node)
{
	t_node	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

// /* ************************************************************************** */
// /*                                                                            */
// /*                                    parser.c                                */
// /*                                                                            */
// /* ************************************************************************** */

// t_parser	*create_parser_node(t_node_type type)
// {
// 	t_parser	*parser;

// 	parser->type = type;
// 	parser->cmd = NULL;
// 	parser->args = NULL;
// 	parser->heredoc_delim = NULL;
// 	parser->redir_file = NULL;
// 	parser->redir_direction = -1;
// 	parser->error = 0;
// 	parser->error_msg = NULL;
// 	parser->next = NULL;
// 	return (parser);
// }

// t_parser	*create_redir_node(t_token_type type)
// {
// 	t_parser	*redir_node;

// 	redir_node = NULL;
// 	if (type == TOKEN_APPEND)
// 	{
// 		redir_node = create_parser_node(NODE_APPEND);
// 		redir_node->redir_direction = OUTPUT;
// 	}
// 	else if (type == TOKEN_REDIR_IN)
// 	{
// 		redir_node = create_parser_node(NODE_REDIR_IN);
// 		redir_node->redir_direction = INPUT;
// 	}
// 	else if (type == TOKEN_HEREDOC)
// 	{
// 		redir_node = create_parser_node(NODE_HEREDOC);
// 		redir_node->redir_direction = INPUT;
// 	}
// 	else if (type == TOKEN_REDIR_OUT)
// 	{
// 		redir_node = create_parser_node(NODE_REDIR_OUT);
// 		redir_node->redir_direction = OUTPUT;
// 	}
// 	return (redir_node);
// }

// void	parse_redirection(t_token *current_token, t_parser **head)
// {
// 	t_parser		*redir;
// 	t_token_type	redir_type;

// 	redir_type = current_token->type;
// 	redir = create_redir_node(redir_type);
// }

// void	parse_simple_command(t_token *current_token, t_parser **head)
// {
// 	while (current_token)
// 	{
// 		if (is_redir_token(current_token->type))
// 			parse_redirection(current_token, head);
// 		else if (current_token->type == TOKEN_WORD)
// 			handle_word_token(current_token, head);
// 		else if (current_token->type == TOKEN_PIPE)
// 		{
// 			parse_pipe(current_token, head);
// 			break ;
// 		}
// 	}
// }

// t_parser	*parse(t_minishell *shell)
// {
// 	// init_parser(shell);
// 	t_token		*current_token;
// 	t_parser	*head;

// 	// create_parser_node(head);
// 	if (!shell->token_list)
// 		return (NULL);
// 	current_token = shell->token_list;
// 	while (current_token && !shell->parser_list->error)
// 	{
// 		parse_simple_command(current_token, &head);
// 		if (shell->parser_list->error)
// 			break ;
// 	}
// 	return (head);
// }

void	free_node(t_node *node)
{
	int		i;

	if (!node)
		return ;
	if (node->cmd)
		free(node->cmd);
	if (node->heredoc_delim)
		free(node->heredoc_delim);
	if (node->redir_file)
		free(node->redir_file);
	i = 0;
	if (node->arg)
	{
		while (node->arg[i])
			free(node->arg[i++]);
		free(node->arg);
		node->arg = NULL;
	}
	free(node);
	node = NULL;
}

void	free_nodes_list(t_node *head)
{
	t_node	*current;
	t_node	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_node(current);
		current = next;
	}
}

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

void	redir_error(t_parser *parser, char *msg, t_node *redir_node)
{
	set_parser_error(parser, msg);
	free_node(redir_node);
}

void	parse_redirection(t_parser *parser, t_node **head)
{
	t_node			*redir;
	t_token_type	redir_type;

	redir_type = parser->current_token->type;
	redir = create_redir_node(redir_type);
	if (!redir)
	{
		set_parser_error(parser, "Failed to create redirection node");
		return ;
	}
	consume_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		if (redir_type == TOKEN_HEREDOC)
			redir_error(parser, "Expected delimiter after '<<'", redir);
		else
			redir_error(parser, "Expected filename after redirection", redir);
		return ;
	}
	if (redir_type == TOKEN_HEREDOC)
		redir->heredoc_delim = ft_strdup(parser->current_token->value);
	else
		redir->redir_file = ft_strdup(parser->current_token->value);
	consume_token(parser);
	add_node_to_back(head, redir);
}

int	add_arg_to_command(char *arg, t_node *cmd_node)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd_node->arg && cmd_node->arg[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
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
	return (1);
}

t_node	*create_command_node(char *cmd, t_node_type type)
{
	t_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->cmd = ft_strdup(cmd);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

void	handle_word_token(t_parser *parser, t_node **head, t_node **first_node)
{
	char	*expanded_arg;

	expanded_arg = expand_var(parser->current_token->value);
	if (!*first_node)
	{
		*first_node = create_command_node(expanded_arg, NODE_CMD);
		add_node_to_back(head, *first_node);
	}
	else
		if (!add_arg_to_command(expanded_arg, *first_node))
			set_parser_error(parser, "Failed to add argument to command");
	free(expanded_arg);
	consume_token(parser);
}

void	parse_simple_command(t_parser *parser, t_node **head)
{
	t_node	*first_node;

	first_node = NULL;
	while (parser->current_token)
	{
		if (is_redir_token(parser->current_token->type))
			parse_redirection(parser, head);
		else if (parser->current_token->type == TOKEN_WORD)
			handle_word_token(parser, head, &first_node);
		else if (parser->current_token->type == TOKEN_PIPE)
		{
			parse_pipe(parser, head);
			break ;
		}
		else
			break ;
	}
}

void	parse_pipe(t_parser *parser, t_node **head)
{
	t_node	*pipe_node;

	pipe_node = create_node(NODE_PIPE);
	if (!pipe_node)
	{
		set_parser_error(parser, "Failed to create pipe node");
		return ;
	}
	add_node_to_back(head, pipe_node);
	consume_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		set_parser_error(parser, "Expected command after pipe");
		return ;
	}
	parse_simple_command(parser, head);
}

t_node	*parse(t_parser *parser)
{
	t_node	*head;

	head = NULL;
	if (!parser->token_list)
		return (NULL);
	while (parser->current_token && !parser->error)
	{
		parse_simple_command(parser, &head);
		if (parser->error)
			break ;
	}
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                      utils.c                               */
/*                                                                            */
/* ************************************************************************** */

void	ft_error(char *msg)
{
	ft_fprintf(2, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/* ************************************************************************** */
/*                                                                            */
/*                            tokenizer_utils.c                               */
/*                                                                            */
/* ************************************************************************** */

char	peek(t_tokenizer *tokenizer)
{	
	return (tokenizer->user_input[tokenizer->position]);
}

void	next_char(t_tokenizer *tokenizer)
{
	tokenizer->position++;
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (value)
		new_token->value = value;
	else
		new_token->value = NULL;
	new_token->type = type;
	// new_token->quote = UNQUOTED;
	new_token->expand = 0;
	new_token->next = NULL;
	return (new_token);
}

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

t_tokenizer	*init_tokenizer(char *input)
{
	t_tokenizer	*tokenizer;

	tokenizer = malloc(sizeof(t_tokenizer));
	if (!tokenizer)
		return (NULL);
	tokenizer->user_input = input;
	tokenizer->position = 0;
	return (tokenizer);
}

void	skip_whitespace(t_tokenizer *tokenizer)
{
	while (peek(tokenizer) && is_whitespace(peek(tokenizer)))
		next_char(tokenizer);
}

/* ************************************************************************** */
/*                                                                            */
/*                                  tokenizer.c                               */
/*                                                                            */
/* ************************************************************************** */

char	*handle_quotes(t_tokenizer *tokenizer, char quote)
{
	int		start;
	int		len;
	char	*word;

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

char	*handle_no_quotes(t_tokenizer *tokenizer)
{
	int		start;
	int		len;
	char	*word;

	start = tokenizer->position;
	while (!is_whitespace(peek(tokenizer))
		&& !is_operator(peek(tokenizer)) && peek(tokenizer))
		next_char(tokenizer);
	len = tokenizer->position - start;
	word = ft_substr(tokenizer->user_input, start, len);
	return (word);
}

char	*get_word(t_tokenizer *tokenizer, int *expand)
{
	char	*chunk;
	char	quote;
	char	*word;
	char	*temp;

	word = ft_strdup("");
	while (peek(tokenizer) && !is_whitespace(peek(tokenizer)) && !is_operator(peek(tokenizer)))
	{
		if (is_quote(peek(tokenizer)))
		{
			quote = peek(tokenizer);
			chunk = handle_quotes(tokenizer, quote);
			if (!chunk)
				return (free(word), NULL);
			if (quote == 34 && ft_strchr(chunk, '$'))
				*expand = 1;
		}
		else
		{
			chunk = handle_no_quotes(tokenizer);
			if (!chunk)
				return (free(word), NULL);
			if (ft_strchr(chunk, '$'))
				*expand = 1;
		}
		temp = ft_strjoin(word, chunk);
		if (!temp)
			return (NULL);
		free(word);
		free(chunk);
		word = temp;
	}
	return (word);
}

t_token	*create_word_token(t_tokenizer *tokenizer)
{
	t_token	*new_token;
	char	*word;
	int		expand;

	expand = 0;
	word = get_word(tokenizer, &expand);
	if (!word)
		return (create_token(TOKEN_ERROR, "Failed to get word"));
	new_token = create_token(TOKEN_WORD, word);
	if (expand)
		new_token->expand = 1;
	free(word);
	return (new_token);
}

t_token	*create_op_token(t_tokenizer *tokenizer)
{
	char	op;

	op = peek(tokenizer);
	next_char(tokenizer);
	if (op == '|')
		return (create_token(TOKEN_PIPE, NULL));
	else if (op == '<' || op == '>')
	{
		if (peek(tokenizer) == op)
		{
			next_char(tokenizer);
			if (op == '<')
				return (create_token(TOKEN_HEREDOC, NULL));
			return (create_token(TOKEN_APPEND, NULL));
		}
		else
		{
			if (op == '<')
				return (create_token(TOKEN_REDIR_IN, NULL));
			return (create_token(TOKEN_REDIR_OUT, NULL));
		}
	}
	return (create_token(TOKEN_ERROR, "Unrecognized operator"));
}

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
	free(tokenizer);
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                      init.c                                */
/*                                                                            */
/* ************************************************************************** */

void	init_shell(t_minishell *shell)
{
	shell->user_input = NULL;
	shell->token_list = malloc(sizeof(t_token));
	if (!shell->token_list)
		ft_error("Token list malloc failed");
	shell->parser = malloc(sizeof(t_parser));
	if (!shell->parser)
		ft_error("Parser struct malloc failed");
	shell->parser_list = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                      main.c                                */
/*                                                                            */
/* ************************************************************************** */

void	get_user_input(char **line)
{
	*line = readline("minishell$ ");
	if (!*line)
	{
		printf("exit\n");
		exit(1);
	}
	add_history(*line);
}

void	free_token_list(t_token *head)
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

int	check_token_error(t_minishell *shell)
{
	t_token	*current;

	current = shell->token_list;
	while (current)
	{
		if (current->type == TOKEN_ERROR)
		{
			printf("%s\n", current->value);
			free_token_list(shell->token_list);
			shell->token_list = NULL;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	cleanup_parser(t_minishell *shell)
{
	free_token_list(shell->token_list);
	if (shell->parser->error_msg)
		free(shell->parser->error_msg);
	free(shell->user_input);
	shell->token_list = NULL;
	shell->parser->error_msg = NULL;
	shell->user_input = NULL;
}

void	reset_main(t_minishell *shell)
{
	free_token_list(shell->token_list);
	free(shell->user_input);
	if (shell->parser_list)
		free_nodes_list(shell->parser_list);
	shell->parser_list = NULL;
	shell->token_list = NULL;
	shell->user_input = NULL;
}

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

void	minishell_loop(t_minishell *shell)
{
	while (1)
	{
		get_user_input(&shell->user_input);
		tokenize(shell->user_input);
		if (check_token_error(shell))
			free(shell->user_input);
		// else
		// {
		// 	shell->parser = init_parser(shell->token_list);
		// 	if (!shell->parser)
		// 	{
		// 		ft_fprintf(STDERR_FILENO, "Failed to initialize parser\n");
		// 		cleanup_parser(shell);
		// 	}
		// 	shell->parser_list = parse(shell->parser);
		// 	if (!shell->parser_list || shell->parser->error)
		// 	{
		// 		ft_fprintf(STDERR_FILENO, "%s\n", shell->parser->error_msg);
		// 		cleanup_parser(shell);
		// 	}
		// }

		print_tokens(shell->token_list);
		// print_nodes(shell->parser_list);

		reset_main(shell);
	}
	// free_nodes_list(shell->parser_list);
}

void	free_shell(t_minishell *shell)
{
	free(shell->token_list);
	free(shell->parser_list);
	free(shell);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*shell;

	(void)argv;
	if (argc != 1)
		return (ft_fprintf(2, "Program does not accept arguments.\n"), 1);
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_fprintf(2, "Shell malloc failed\n"), 1);
	init_shell(shell);
	signal(SIGINT, signal_handler);
	minishell_loop(shell);
	free_shell(shell);
	return (0);
}
