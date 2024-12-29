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

char **lexer(char *str)
{
	return (ft_split(str, ' '));
}

int main(int argc, char **argv)
{
	(void)argc;

	char **line = lexer(argv[1]);

	int i = 0;
	while (line)
		printf("%s\n", line[i]);
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