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

# include <readline/readline.h> // readline, add_history, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>
# include <stdio.h> // printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // write, access, open, read, close, fork, pipe, isatty, ttyname, ttyslot, getcwd, chdir, unlink, execve, dup, dup2 
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
# include "library/ft_printf/inc/ft_printf.h"
# include "library/get_next_line/get_next_line.h"
# include "library/get_next_line/get_next_line_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*                               for tokenizer                                */
/*                                                                            */
/* ************************************************************************** */

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
}	t_parser;

typedef struct s_node
{
	t_node_type		type;
	char			*cmd;
	char			**arg;
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

#endif