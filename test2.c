#include "minishell.h"

#define METACHAR "|&;<>() \t\n"
#define OPERATOR "|&<>()"

typedef struct s_scanner
{
	const char	*start;
	const char	*current;
}	t_scanner;

typedef enum e_tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_LESS,
	TOK_GREAT,
	TOK_DLESS,
	TOK_DGREAT,
	TOK_OR_IF,
	TOK_AND_IF,
	TOK_AND,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_EOF,
	TOK_ERROR,
}	t_tok_type;

typedef struct s_token
{
	t_tok_type	type;
	char		*value;
}	t_token;

typedef struct s_minishell
{
	// t_cmd			*syntax_tree;
	int				std_fds[3];
	int				pid;
	int				pid_left;
	int				pid_right;
	int				status;
	int				exit_code;
	char			**env;
	char			**env_storage;
	// t_env_lst		*env_lst;
	// t_list			*garbage;
	// t_exec_cmd		*e_cmd;
	struct termios	term;
}	t_minishell;

void	*to_gc_lst(void *content)
{
	static t_list	*head_lst;
	t_list			**head_ptr;

	head_ptr = &head_lst;
	ft_lstadd_back(head_ptr, ft_lstnew(content));
	return (content);
}

/*
*	@brief free all the contents from the garbage collector list
*/
void	free_gc_lst(void)
{
	static t_list	*head_lst;
	t_list			**head_ptr;

	head_ptr = &head_lst;
	ft_lstclear(head_ptr, free);
}


void	init_scanner(t_scanner *scanner, const char *line)
{
	scanner->current = line;
	scanner->start = line;
}

/* 
*	@brief 1. auxiliary function for make_operator_token()
*	@brief 2. checks if next char is expected char
*	@param  expect character to checked
*	@param  iftrue token type if true
*	@param  iffalse token type if false
*	@param  s scanner struct
*	@return 1. if false, returns iffalse token type
*	@return 2. if true, returns iftrue token type 
			and moves current front by 1
*/
t_token	match_next(char expect, t_tok_type iftrue, t_tok_type iffalse,
	t_scanner *s)
{
	t_token		token;
	const char	*tok_values[] = {
	[TOK_DLESS] = "<<", [TOK_LESS] = "<",
	[TOK_DGREAT] = ">>", [TOK_GREAT] = ">",
	[TOK_OR_IF] = "||", [TOK_PIPE] = "|",
	[TOK_AND_IF] = "&&", [TOK_ERROR] = "&",
	};

	if (*s->current == '\0')
		token.type = iffalse;
	if (*s->current != expect)
		token.type = iffalse;
	else
	{
		token.type = iftrue;
		s->current++;
	}
	token.value = to_gc_lst(ft_strdup(tok_values[token.type]));
	return (token);
}

/*
*	@brief give each of the string a type and assign to value
*	@param type token type
*	@param string the string
*	@return the token with its type and value
*/
t_token	make_token(t_tok_type type, char *string)
{
	t_token	new_token;

	new_token.type = type;
	new_token.value = string;
	return (new_token);
}

/*
*	@brief check the quote type
*	@param scanner the scanner struct
*	@param quote_type the quote type to check
*	@return 0 if is the quote type
*	@return , -1 if not the quote type
*/
int	quotes(t_scanner *scanner, char quote_type)
{
	while (*scanner->current != '\0' && *scanner->current != quote_type)
		scanner->current++;
	if (*scanner->current == '\0' && *scanner->current != quote_type)
		return (-1);
	return (0);
}

/*
*	@brief make a word token
*	@param scanner the scanner struct
*	@return if success the token with its type and value
*	@return if fail the error token upon seeing a unclosed quotes
*	@note 1. check the qutoe whether it is closed or not
*	@note 	 , if closed, then check word
*	@note 2. METACHAR is defined as "|&;<>() \t\n"
*/
t_token	make_word_token(t_scanner *scanner)
{
	char	*word;
	char	c;

	scanner->current--;
	while (!ft_strchr(METACHAR, *scanner->current))
	{
		c = *(scanner->current++);
		if (c == '\'' || c == '"')
		{
			if (quotes(scanner, c) == -1)
				return (make_token(TOK_ERROR, "Unclosed Quotes"));
			scanner->current++;
		}
	}
	word = ft_substr(scanner->start, 0, (scanner->current - scanner->start));
	return (make_token(TOK_WORD, word));
}

/*
*	@brief make token and check if the next character is the same
*	@param curr the current string
*	@param scanner the scanner struct 
*	@note 1. for single character operators it returns its type
*	@note 2. for double character ones it checks it with match_next()
*	@note 3. the function will not reach the last line 
*/
t_token	make_operator_token(char curr, t_scanner *scanner)
{
	if (curr == '(')
		return (make_token(TOK_LPAREN, to_gc_lst(ft_strdup("("))));
	if (curr == ')')
		return (make_token(TOK_RPAREN, to_gc_lst(ft_strdup(")"))));
	if (curr == '<')
		return (match_next('<', TOK_DLESS, TOK_LESS, scanner));
	if (curr == '>')
		return (match_next('>', TOK_DGREAT, TOK_GREAT, scanner));
	if (curr == '|')
		return (match_next('|', TOK_OR_IF, TOK_PIPE, scanner));
	if (curr == '&')
		return (match_next('&', TOK_AND_IF, TOK_ERROR, scanner));
	return (make_token(TOK_ERROR, NULL));
}

/* 
*	@brief Scans the string and returns the next word/operator(token)
*	@param scanner the scanner struct
*	@return if fail the error token upon seeing a unexpected character 
*	@return , if success the token with its type and value
*	@note 1. OPERATOR is defined as "|&<>()"
*	@note 2. METACHAR is defined as "|&;<>() \t\n"
*/
t_token	scan_token(t_scanner *scanner)
{
	char	curr;

	while (ft_isspace(*scanner->current))
		scanner->current++;
	scanner->start = scanner->current;
	if (*scanner->current == '\0')
		return (make_token(TOK_EOF, "newline"));
	curr = *scanner->current;
	scanner->current++;
	if (ft_strchr(OPERATOR, curr))
		return (make_operator_token(curr, scanner));
	if (!ft_strchr(METACHAR, curr))
		return (make_word_token(scanner));
	return (make_token(TOK_ERROR, NULL));
}

// static void	init_shell(t_minishell *m_shell, char *envp[])
// {
// 	ft_bzero(m_shell, sizeof(t_minishell));
// 	save_std_fds(m_shell);
// 	m_shell->env = envp;
// 	init_signal(m_shell);
// 	change_shlvl(m_shell);
// 	store_env(m_shell);
// 	remove_env_var(m_shell, "OLDPWD");
// }

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_minishell	m_shell;

	((void)argc, (void)argv);
	// init_shell(&m_shell, envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			(ft_printf("exit\n"));
		if (!*line)
		{
			free(line);
			continue ;
		}
		// get_exit_code_ctrl_c(&m_shell);
		// add_history(line);
		// m_shell.syntax_tree = parse(line, &m_shell);
		// if (m_shell.syntax_tree != NULL)
		// {
		// 	execute(&m_shell);
		// }
		free(line);
	}
	return (0);
}