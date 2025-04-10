/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:13:19 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/18 19:13:19 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// current checklist for parser:
// - syntax error
//   - tokens DONE
//   - parser DONE
// - handle quotes DONE-ish -> need to handle words that are
//		joined to the quoted word (eg: "hello"world)
// - empty input -> prints (null) DONE
// - functions too long DONE

// - handle expansion DONE? waiting for execution to complete i think
// - handle exit codes
// - handle signals -> what's the diff between interactive mode and non-interactive?
// - handle history DONE?
// - handle escape characters NOT NEEDED BY SUBJECT.PDF
// - handle auto-continue (aka tab)? NOT COMPULSORY I THINK

/* as of 23 Feb
	- unquoted words joined to quoted words (eg: "hi"world) -> also has LEAKS
	- second word onwards after the redir is treated as an arg to the command (first word)
		(eg: echo hi > input ls) -> should be correct or not? need to test more cases
*/

// void	print_tokens(t_token *head);
// void	print_nodes(t_node *head);

void	main_parse(t_minishell *shell)
{
	shell->parser = init_parser(shell->token_list);
	if (!shell->parser)
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize parser\n");
		cleanup_parser(shell);
	}
	shell->parser_list = parse(shell->parser);
	if (shell->parser->error || !shell->parser_list)
	{
		ft_fprintf(STDERR_FILENO, "%s\n", shell->parser->error_msg);
		cleanup_parser(shell);
	}
}

// gets the user input and also enables
// the use of arrow keys to navigate the input
// ** current exit status is wrong
char	*get_user_input(char **line)
{
	*line = readline("minishell$ ");
	if (!*line)
	{
		printf("exit\n");
		exit(1);
	}
	if (!**line)
	{
		free(*line);
		*line = NULL;
		return (NULL);
	}
	add_history(*line);
	return (*line);
}

// void	parent_execution(t_minishell *shell)
// {
// 	t_node	*current_node;

// 	current_node = shell->parser_list;
// 	if (!current_node)
// 		return ;
// 	if (current_node->type == NODE_COMMAND)
// 	{
// 		printf("entered parent_execution\n");
// 		if (check_builtin(current_node->cmd))
// 		{
// 			shell->exit = run_builtin(current_node->arg, shell);
// 			// printf("%d\n", shell->exit);
// 			return ;
// 		}
// 	}
// 	// execute_fork(shell, current_node);
// }

void	minishell_loop(t_minishell *shell)
{
	char	*line;

	while (1)
	{
		line = NULL;
		if (!get_user_input(&line))
			continue ;
		shell->user_input = line;
		shell->token_list = tokenize(shell->user_input);
		if (check_token_error(shell))
		{
			free(shell->user_input);
			shell->user_input = NULL;
			continue ;
		}
		else
			main_parse(shell);
		// printf("parsing complete\n");
		parent_execution(shell);
		// run_builtin(shell->parser_list->arg, shell);
			//do something later

		// print_tokens(shell->token_list);
		// print_nodes(shell->parser_list);
		reset_main(shell);
	}
	free_nodes_list(shell->parser_list);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	(void)argv;
	if (argc != 1)
		return (printf("Program does not accept arguments.\n"), 1);
	init_shell(&shell, envp);
	init_env(&shell, envp);
	signal(SIGINT, signal_handler);
	minishell_loop(&shell);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                        test.c                              */
/*                                                                            */
/* ************************************************************************** */

/* Debug function to print tokens */
// void print_tokens(t_token *head)
// {
//     t_token *current = head;
//     const char *type_strings[] = {
//         "WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
//         "HEREDOC", "APPEND", "ERROR", "EOF"
//     };
// 	const char *quote_strings[] = {
// 		"NO_QUOTE", "DOUBLE_QUOTE", "SINGLE_QUOTE", "UNTERMINATED QUOTE"
// 	};

//     while (current) {
//         printf("Type: %-10s", type_strings[current->type]);
//         if (current->value)
//             printf(" Value: '%s'", current->value);
// 		if (current->quote)
// 			printf(" Quote: %s", quote_strings[current->quote]);
// 		if (current->expand)
// 			printf(" Expand: %d", current->expand);
//         printf("\n");
//         current = current->next;
//     }
// }

// void print_nodes(t_node *head)
// {
// 	t_node	*current;

// 	current = head;
// 	while (current)
// 	{
// 		if (current->type == NODE_COMMAND)
// 		{
// 			printf("Command: %s\n", current->cmd);
// 			if (current->arg)
// 			{
// 				for (int i = 0; current->arg[i]; i++)
// 					printf("Arg: %s\n", current->arg[i]);
// 			}
// 		}
// 		else if (current->type == NODE_PIPE)
// 			printf("Pipe\n");
// 		else if (current->type == NODE_REDIR_IN)
// 			printf("Redir in: %s\n", current->redir_file);
// 		else if (current->type == NODE_REDIR_OUT)
// 			printf("Redir out: %s\n", current->redir_file);
// 		else if (current->type == NODE_HEREDOC)
// 			printf("Heredoc: %s\n", current->heredoc_delim);
// 		else if (current->type == NODE_APPEND)
// 			printf("Append: %s\n", current->redir_file);
// 		current = current->next;
// 	}
// }

/* Test cases */
// void test_parser(char **envp)
// {
//     // Test case structure
//     struct test_case {
//         char *input;
//         char *description;
//     };

    // Define test cases
    // struct test_case tests[] = {
    //     {"ls -l", "Simple command"},
    //     {"ls -l | grep foo", "Simple pipe"},
    //     {"ls -l|grep foo", "Pipe without spaces"},
    //     {"cat < input.txt", "Input redirection"},
    //     {"echo hello > output.txt", "Output redirection"},
    //     {"echo hello >> output.txt", "Append redirection"},
    //     {"cat << EOF", "Heredoc"},
    //     {"ls -l | grep foo > output.txt", "Pipe with redirection"},
    //     {"ls -l|grep foo>output.txt", "Pipe with redirection, no spaces"},
    //     {"cat < input.txt | grep foo > output.txt", "Complex command"},
    //     {"ls     -l     |    grep    foo", "Multiple spaces"},
    //     {"ls", "Single command"},
    //     {"", "Empty input"},
	// 	{"ls -l | grep foo | wc -l", "Multiple pipes"},
	// 	{"ls -l | grep foo | wc -l > output.txt",
	// 			"Multiple pipes with redirection"},
    //     {NULL, NULL}
    // };

    // int i = 0;
    // while (tests[i].input != NULL)
    // {
    //     printf("\nTest %d: %s\n", i + 1, tests[i].description);
    //     printf("Input: '%s'\n", tests[i].input);
    //     printf("Output AST:\n");

    //     // Tokenize
    //     t_token *tokens = tokenize(tests[i].input);
    //     if (!tokens)
    //     {
    //         printf("Tokenization failed!\n");
    //         i++;
    //         continue;
    //     }

    //     // Print tokens for debugging
    //     printf("Tokens:\n");
    //     print_tokens(tokens);
	// }

	// char	*input = "echo $HOME";
	// t_token *tokens = tokenize(input);
	// t_parser	parser = {
	// 	.current_token = tokens,
	// 	.token_list = tokens,
	// 	.error = 0,
	// 	.error_msg = NULL,
	// 	.env = envp
	// };
	// t_node*	head = parse(&parser);

	// while (head)
	// {
	// 	head = head->next;
	// }
    //     // Parse
    //     t_ast *ast = parse(tokens);
    //     if (!ast)
    //     {
    //         printf("Parsing failed!\n");
    //         free_tokens(tokens);
    //         i++;
    //         continue;
    //     }
	// 	printf("\nPARSER\n");
    //     // Print AST
    //     print_ast(ast, 0);

    //     // Cleanup
    //     free_ast_node(ast);
    //     free_tokens(tokens);
    //     i++;
    // }
// }

// int main(int ac, char** argv, char** envp)
// {
// 	(void)ac; (void)argv;
//     test_parser(envp);
//     return 0;
// }
