/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:42:02 by ayeap             #+#    #+#             */
/*   Updated: 2024/11/28 14:42:02 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# include "library/inc/libft.h"
// # include "library/ft_printf/inc/ft_printf.h"
// # include "library/get_next_line/get_next_line.h"
// # include "library/get_next_line/get_next_line_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*                               for tokenizer                                */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_quote_type
{
	NO_QUOTE,
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	UNTERMINATED_QUOTE
}	t_quote_type;

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
	t_quote_type	quote;
	int				expand;
	struct s_token	*next;
}	t_token;

typedef struct s_tokenizer
{
	char	*user_input;
	int		position;
}	t_tokenizer;

/* ************************************************************************** */
/*                                                                            */
/*                                  for parser                                */
/*                                                                            */
/* ************************************************************************** */

typedef enum s_node_type
{
	NODE_COMMAND,
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

typedef struct s_minishell
{
	char		*user_input;
	char		**env; // can remove
	t_token		*token_list;
	t_node		*parser_list; // pointer to the first node of the parser
	t_parser	*parser; // 
	// int			exit_status;
}	t_minishell;

// expansion.c
char		*get_env_var(char **value, char *var_start, char *result, int *i);
char		*expand_var(char *value);

// free_nodes.c
void		free_node(t_node *head);
void		free_nodes_list(t_node *head);

// main_utils.c
void		init_shell(t_minishell *shell, char **env);
void		reset_main(t_minishell *shell);
int			check_token_error(t_minishell *shell);
void		cleanup_parser(t_minishell *shell);

// main.c
void		minishell_loop(t_minishell *shell);
void		main_parse(t_minishell *shell);
char		*get_user_input(char **line);

// parse_redir.c
t_node		*create_redir_node(t_token_type type);
void		redir_error(t_parser *parser, char *msg, t_node *redir_node);
void		parse_redirection(t_parser *parser, t_node **head);

// parse_simple_command.c
int			add_arg_to_command(char *arg, t_node *command);
t_node		*create_command_node(char *cmd, t_node_type type);
void		handle_word_token(t_parser *parser, t_node **head,
				t_node **first_node);
void		parse_simple_command(t_parser *parser, t_node **head);

// parser_utils.c
int			is_redir_token(t_token_type type);
void		consume_token(t_parser *parser);
void		set_parser_error(t_parser *parser, char *msg);
void		add_node_to_back(t_node **head, t_node *new_node);
t_parser	*init_parser(t_token *token_list);

// parser.c
t_node		*create_node(t_node_type type);
void		parse_pipe(t_parser *parser, t_node **head);
t_node		*parse(t_parser *parser);

// signals.c
void		signal_handler(int signum);

// tokenizer_create_tokens.c
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_token);

// tokenizer_quotes.c
t_token		*create_quote_token(t_tokenizer *tokenizer);
char		*handle_quotes(t_tokenizer *tokenizer, char quote);

// tokenizer_unquoted.c
t_token		*create_unquoted_token(t_tokenizer *tokenizer);
char		*handle_no_quotes(t_tokenizer *tokenizer);

// tokenizer_utils.c
char		peek(t_tokenizer *tokenizer);
void		next_char(t_tokenizer *tokenizer);
void		skip_whitespace(t_tokenizer *tokenizer);
t_tokenizer	*init_tokenizer(char *input);
void		free_token_list(t_token *head);

// tokenizer.c
t_token		*create_word_token(t_tokenizer *tokenizer);
t_token		*create_op_token(t_tokenizer *tokenizer);
t_token		*tokenize(char *user_input);

// utils.c
int			is_whitespace(char c);
int			is_operator(char c);

#endif