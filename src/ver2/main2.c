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

typedef struct s_parser_node {
	t_node_type				type;
	char					*cmd;
	char					**args;
	char					*heredoc_delim;
	char					*redir_file;
	t_direction				redir_direction;
	struct s_parser_node	*next;
}	t_parser;

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
	t_parser	*parser_list;
}	t_minishell;

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
				return (free(word), create_token(TOKEN_ERROR, "Unterminated quote"));
			if (quote == 34 && ft_strchr(chunk, '$'))
				*expand = 1;
		}
		else
		{
			chunk = handle_no_quotes(tokenizer);
			if (!chunk)
				return (free(word), create_token(TOKEN_ERROR, "Failed to create word token"));
			if (ft_strchr(chunk, '$'))
				expand = 1;
		}
		temp = ft_strjoin(word, chunk);
		if (!temp)
			return (create_token(TOKEN_ERROR, "Temp malloc failed"));
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
	shell->parser_list = malloc(sizeof(t_parser));
	if (!shell->parser_list)
		ft_error("Parser list malloc failed");
}

/* ************************************************************************** */
/*                                                                            */
/*                                      main.c                                */
/*                                                                            */
/* ************************************************************************** */

void	get_user_input(char *line)
{
	line = readline("minishell$ ");
	if (!line)
	{
		printf("exit\n");
		exit(1);
	}
	add_history(line);
}

void	minishell_loop(t_minishell *shell)
{
	while (1)
	{
		get_user_input(shell->user_input);
		tokenize(shell->user_input);
	}
}

void	free_shell(t_minishell *shell)
{
	free(shell->token_list);
	free(shell->parser_list);
	free(shell);
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
	// signal(SIGINT, signal_handler);
	minishell_loop(shell);
	free_shell(shell);
	return (0);
}
