/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:03 by ayeap             #+#    #+#             */
/*   Updated: 2024/11/28 14:43:03 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
1. parsing input
use readline() to print the "minishell $" and read the input into a variable
take that variable and ft_split() with is_whitespace() as delimiters (define it in header file?)
parse each pointer to assign a token value -> create a struct to hold the value

checking for edge cases
when using redir
	-> must have something after as delimiter
when using pipes
	-> cannot have multiple consecutive pipes (eg: "||")

2. history (pressing the up and down keys in terminal)

3. auto-complete (pressing tab completes with the file name)
*/

// functions needed ?
// - handle_quotes
// - handle_expansion ($)

// iterate through the string and return the last letter before the end quote
// char *handle_quotes(char *argv)
// {
	
// }

char *ft_strtok(char *str, char delim)
{
	static char *next_token;
	char *token_start;
	char *token_end;
	
	if (str != NULL)
		next_token = str;
	if (!next_token)
		return (NULL);
	token_start = next_token;
	while (*token_start && *token_start == delim)
		token_start++;
	if (*token_start == '\0')
	{
		next_token = NULL;
		return (NULL);
	}
	token_end = token_start;
	while (*token_end && *token_end != delim)
		token_end++;
	if (*token_end)
	{
		*token_end = '\0';
		next_token = token_end + 1;
	}
	else
		next_token = NULL;
	return (token_start);
}

char **lexer(char *line)
{
	// return (ft_split(argv, ' '));
	int i = 0;
	char **arr;

	arr = malloc(sizeof(char *) * MAX_TOKENS);
	if (!arr)
		return (NULL);
	char *token = ft_strtok(line, ' ');
	while (token != NULL && i < MAX_TOKENS - 1)
	{
		// if (line == 34 && line == 39)
		// 	handle_quotes(line, )
		// else
			arr[i++] = token;
			token = ft_strtok(NULL, ' ');
	}
	arr[i] = NULL;
	return (arr);
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

int main(int argc, char **argv)
{
	(void)argv;

	if (argc != 1)
		return (printf("This program doesn't accept arguments\n"), 1);
	signal(SIGINT, sigint_handler);
	while (1)
	{
		char *line = readline("minishell$ ");
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		char **tokens = lexer(line);
		int i = 0;
		while (tokens[i])
			printf("%s\n", tokens[i++]);
		free(tokens);
		free(line);
		// signal(SIGINT, sigint_handler);
	}
	return (0);
}

// #define MAX_TOKENS 64
// #define TOKEN_DELIMITERS " \t\r\n\a"

// void parse_and_execute(char *line) {
// 	char *tokens[MAX_TOKENS];
// 	int token_count = 0;
// 	char *token = strtok(line, TOKEN_DELIMITERS);

// 	while (token != NULL && token_count < MAX_TOKENS - 1) {
// 		tokens[token_count++] = token;
// 		token = strtok(NULL, TOKEN_DELIMITERS);
// 	}
// 	tokens[token_count] = NULL;

// 	if (token_count == 0) {
// 		return; // Empty input
// 	}

// 	pid_t pid = fork();
// 	if (pid == 0) {
// 		// Child process
// 		if (execvp(tokens[0], tokens) == -1) {
// 			perror("minishell");
// 		}
// 		exit(EXIT_FAILURE);
// 	} else if (pid < 0) {
// 		// Error forking
// 		perror("minishell");
// 	} else {
// 		// Parent process
// 		int status;
// 		waitpid(pid, &status, 0);
// 	}
// }

// int main() {
// 	while (1) {
// 		char *line = readline("minishell$ ");
// 		if (!line || strcmp(line, "exit") == 0) {
// 			free(line);
// 			break;
// 		}
// 		parse_and_execute(line);
// 		free(line);
// 	}
// 	return 0;
// }

// int main(int argc, char **argv) {
//     // if (argc > 1) {
//     //     printf("Welcome, %s!\n", argv[1]);
//     // } else {
//     //     printf("Welcome, user!\n");
//     // }
// 	(void)argc;
// 	(void)argv;

//     while (1) {
//         char *line = readline("minishell$ ");
//         if (!line || strcmp(line, "exit") == 0) {  // Exit on EOF or "exit"
//             free(line);
//             break;
//         }
//         // printf("You entered: %s\n", line);
//         free(line);
//     }

//     return 0;
// }

// to trigger heredoc
// [COMMAND] << [DELIMITER]
// eg: cat << END