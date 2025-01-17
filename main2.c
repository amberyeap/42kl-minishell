#include "minishell.h"

# define DELIMITERS " \t\n"

typedef enum e_token_type
{
	WORD, // commands like 'echo', 'ls', 'cat', etc.
	PIPE, // '|'
	REDIR_IN, // '<'
	HEREDOC, // '<<'
	REDIR_OUT, // '>'
	APPEND, // '>>'
}	t_token_type;

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_lexer *next;
}	t_token;

typedef struct s_command
{
	char **args;
	char *input;
	char *output;
	struct s_command *next;
}	t_command;

typedef struct s_minishell
{
	t_command *commands;
	t_token *tokens;
}	t_minishell;

int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
char **lexer(char *line)
{

}

int minishell_loop(t_minishell *shell)
{
	char *line;
	char **tokens;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		// if (ft_strlen(line) > 0)
		// 	add_history(line);
		tokens = lexer(line);
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	t_minishell shell;
	char *line;

	if (argc > 1)
		return (1, printf("This program doesn't take any arguments.\n"));
	minishell_loop(&shell);
	return (0);
}