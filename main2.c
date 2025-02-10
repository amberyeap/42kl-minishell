#include "minishell.h"

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

typedef struct s_parser
{
	t_token		*current_token;
	t_token		*token_list;
	int			error;
	char		*error_msg;
}	t_parser;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR,
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	bool			needs_expansion; // for variable expansion ($)
	struct s_node	*left;
	struct s_node	*right;
	union 
	{
		struct s_command_node	command;
		struct s_pipe_node		pipe;
		struct s_redir_node		redir;
	};
}	t_node;

struct s_command_node
{
	char		**args;
};

struct s_pipe_node
{
	struct s_node	*left;
	struct s_node	*right;
};

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}	t_redir_type;

typedef enum e_redir_direction
{
	INPUT,
	OUTPUT
}	t_redir_direction;

struct s_redir_node
{
	char					*filename;
	char					*heredoc_delim;
	bool					heredoc;
	t_redir_type		redir_type;
	t_redir_direction	direction;
};

typedef struct s_minishell
{
	char		*user_input;
	char		**env;
	t_token		*token_list;
	t_parser	*parser;
	t_node		*ast;
}	t_minishell;

