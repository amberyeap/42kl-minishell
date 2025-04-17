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
// - handle signals -> what's the diff between interactive mode 
//		and non-interactive?
// - handle history DONE?
// - handle escape characters NOT NEEDED BY SUBJECT.PDF
// - handle auto-continue (aka tab)? NOT COMPULSORY I THINK

/* as of 23 Feb
	- unquoted words joined to quoted words (eg: "hi"world)
		-> also has LEAKS
	- second word onwards after the redir is treated as an 
		arg to the command (first word)
		(eg: echo hi > input ls) -> should be correct or not? 
			need to test more cases
*/

// void	print_tokens(t_token *head);
// void	print_nodes(t_node *head);

void	main_parse(t_minishell *shell)
{
	shell->parser = init_parser(shell);
	if (!shell->parser)
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize parser\n");
		shell->exit = 1;
		cleanup_parser(shell);
		return ;
	}
	shell->parser_list = parse(shell->parser);
	if (shell->parser->error || !shell->parser_list)
	{
		ft_fprintf(STDERR_FILENO, "%s\n", shell->parser->error_msg);
		shell->exit = 2;
		cleanup_parser(shell);
		return ;
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
		exit(0);
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
			shell->exit = 2;
			free(shell->user_input);
			shell->user_input = NULL;
			continue ;
		}
		else
			main_parse(shell);
		if (shell->parser_list)
			parent_execution(shell);
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
