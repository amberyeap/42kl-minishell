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

// typedef struct s_tokenizer {
// 	char	*user_input
// 	int		position; // what is position for??
// 	t_token	*head;
// }	t_tokenizer;

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
	new_token->quote = UNQUOTED;
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

// char	*get_word(char *user_input, int *pos, int *expand)
// {
// 	char	*word;
// 	char	quote;

// 	while (user_input[*pos] && !is_whitespace(user_input[*pos]) && !is_operator(user_input[*pos]))
// 	{
// 		if (is_quote(user_input[*pos]))
// 		{
// 			quote = user_input[*pos];
// 			word = handle_quotes(user_input, pos, quote);
// 		}
// 		else
// 			word = handle_no_quotes(user_input, pos);
		
// 	}
// }

t_token	*create_word_token(char *user_input, int *pos)
{
	t_token *new_token;
	char	*word;
	int		expand;
	char	quote;
	char	*chunk;

	word = ft_strdup("");
	if (!word)
		create_token(TOKEN_ERROR, "Word malloc failed");
	// word = get_word(user_input, pos, &expand);
	while (user_input[*pos] && !is_whitespace(user_input[*pos]) && !is_operator(user_input[*pos]))
	{
		if (is_quote(user_input[*pos]))
		{
			quote = user_input[*pos];
			chunk = handle_quotes(user_input, pos);
			if (quote)
		}
	}
	
}

t_token	*create_op_token(char *user_input, int *pos)
{
	char	op;

	op = user_input[*pos];
	*pos += 1;
	if (op == '|')
		return (create_token(TOKEN_PIPE, NULL));
	else if (op == '<' || op == '>')
	{
		if (user_input[*pos] == op)
		{
			*pos +=1;
			if (op == '<')
				return (create_token(TOKEN_HEREDOC, NULL));
			return (create_token(TOKEN_APPEND, NULL));
		}
		if (op == '<')
			return (create_token(TOKEN_REDIR_IN, NULL));
		return (create_token(TOKEN_REDIR_OUT, NULL));
	}
	return (create_token(TOKEN_ERROR, "Unrecognised operator"));
}

void	tokenize(char *user_input, t_token *head)
{
	t_token	*new_token;
	int		i;

	i = 0;
	while (user_input[i])
	{
		while (is_whitespace(user_input[i]))
			i++;
		if (user_input[i] == '\0')
			break ;
		if (is_operator(user_input[i]))
			new_token = create_op_token(user_input, &i);
		else
			new_token = create_word_token(user_input, &i);
		add_token(&head, new_token);
		free(new_token);
		i++;
	}
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
		tokenize(shell->user_input, shell->token_list);
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