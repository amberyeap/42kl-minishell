/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:47:50 by ayeap             #+#    #+#             */
/*   Updated: 2025/01/26 16:47:50 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_minishell
{
	char *user_input;
	char **env;
	t_lexer *lexer_list;
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

int ft_error(char *message)
{
	printf("Error: %s\n", message);
	exit(EXIT_FAILURE);
}

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

int check_token(char *str, int i)
{
	if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (HEREDOC); // need to i++ to move the pointer?
		return (REDIR_IN);
	}
	else if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (APPEND); // need to i++?
		return (REDIR_OUT);
	}
	return (0);
}

int	handle_token(t_minishell *shell, int i)
{
	int token_type;

	token_type = check_token(shell->user_input, i);
	if (token_type == PIPE)
		add_lexer(shell->lexer_list, NULL, PIPE);
	else if (token_type == REDIR_IN)
		add_lexer(shell->lexer_list, NULL, REDIR_IN);
	else if (token_type == HEREDOC)
	{
		add_lexer(shell->lexer_list, NULL, HEREDOC);
		return (2);
	}
	else if (token_type == REDIR_OUT)
		add_lexer(shell->lexer_list, NULL, REDIR_OUT);
	else if (token_type == APPEND)
	{
		add_lexer(shell->lexer_list, NULL, APPEND);
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
		add_lexer(lexer_list, ft_substr(str, i, j), WORD);
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
	return (0);
}