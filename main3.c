#include "minishell.h"

#define WHTIESPACE " \t\n"
#define METACHARS "|&;<>() \t\n"
#define OPERATORS "|<>"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	HEREDOC,
	REDIR_OUT,
	APPEND,
	ERROR,
}	t_token_type;

typedef struct s_lexer
{
	char *start;
	char *current;
}	t_lexer;

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_token *next;
	struct s_token *prev;
}	t_token;

typedef struct s_minishell
{
	char	*user_input;
	char	**env;
	t_token	*lexer_list;
}	t_minishell;

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

void	ft_error(char *message)
{
	printf("Error: %s\n", message);
	exit(EXIT_FAILURE);
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	printf("entered create_token\n");
	token = malloc(sizeof(t_token));
	if (!token)
		ft_error("Memory allocation failed");
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token_to_back(t_token *head, char *value, t_token_type type)
{
	t_token	*current;
	t_token	*new_token;

	printf("entered add_token_to_back\n");
	current = head;
	new_token = create_token(value, type);
	if (!head)
	{
		head = new_token;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

int	check_token(char *str, int i)
{
	printf("entered check_token\n");
	if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (HEREDOC);
		return (REDIR_IN);
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (APPEND);
		return (REDIR_OUT);
	}
	return (0);
}

int	make_operator_token(t_minishell *shell, int i)
{
	int	token_type;

	printf("entered make_operator_token\n");
	token_type = check_token(shell->user_input, i);
	if (token_type == REDIR_IN)
		add_token_to_back(shell->lexer_list, NULL, REDIR_IN);
	else if (token_type == HEREDOC)
	{
		add_token_to_back(shell->lexer_list, NULL, HEREDOC);
		return (2);
	}
	else if (token_type == REDIR_OUT)
		add_token_to_back(shell->lexer_list, NULL, REDIR_OUT);
	else if (token_type == APPEND)
	{
		add_token_to_back(shell->lexer_list, NULL, APPEND);
		return (2);
	}
	return (1);
}

int	check_quotes(char *str, int i)
{
	int	j;

	printf("entered check_quotes\n");
	j = 1; // to start after the quote
	while (str[i + j] && str[i + j] != str[i])
		j++;
	if (!str[i + j])
		return (-1);
	return (j);
}

int make_word_token(t_minishell *shell, int i)
{
	int j;
	char *str;

	printf("entered make_word_token\n");
	str = shell->user_input;
	j = 0;
	while (str[i + j] && !check_token(str, i + j))
	{
		if (str[i + j] == 34 || str[i + j] == 39)
			j += check_quotes(str, i + j);
		if (is_whitespace(str[i + j]))
			break ;
		else
			j++;
	}
	if (j > 0)
		create_token(ft_substr(str, i, j), WORD);
	return (j);
}

int 	tokenize(t_minishell *shell)
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
			j += make_operator_token(shell, i);
		else
			j += make_word_token(shell, i);
		if (j < 0)
			return (0);
		i += j;
	}
	return (1);
}

void init_shell(t_minishell *shell, char **env)
{
	shell->env = env;
	shell->lexer_list = malloc(sizeof(t_token));
	if (!shell->lexer_list)
		ft_error("Memory allocation failed");
}

void	print_lexer(t_minishell *shell)
{
	t_token	*current;

	current = shell->lexer_list;
	while (current)
	{
		printf("Token: %s\n", current->value);
		printf("Type: %d\n", current->type);
		current = current->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	t_minishell shell;

	(void)argv;
	if (argc != 1)
		return(printf("Program does not accept arguments.\n"), 1);
	init_shell(&shell, envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (!tokenize(&shell))
			ft_error("Tokenize failed");
		print_lexer(&shell);
	}
}

