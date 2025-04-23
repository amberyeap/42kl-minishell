/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 08:01:54 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/23 20:35:40 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
1. if there's only one arg after 'exit'
	- check if it's a string
	- if not string, print the number
2. if there's more than one arg after 'exit'
	- check if the first arg is string, exit code is 2
	- if the first arg is a number, exit code is 1
*/
int	ft_exit(t_minishell *data, char **args)
{
	ft_fprintf(STDERR_FILENO, "exit\n");
	if (length(args) == 2)
	{
		if (!ft_strisnum(args[1]))
		{
			ft_fprintf(STDERR_FILENO, "minishell: exit: ");
			ft_fprintf(STDERR_FILENO,
				"%s: numeric argument required\n", args[1]);
			exit(2);
		}
		else
			exit(ft_atoi(args[1]) % 256);
	}
	if (length(args) > 2)
	{
		ft_fprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		if (!ft_strisnum(args[1]))
			data->exit = 2;
		else
			data->exit = 1;
		return (data->exit);
	}
	exit(0);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // Helper function to test ft_exit
// void test_exit(char **args)
// {
// 	t_minishell data;
// 	memset(&data, 0, sizeof(t_minishell)); // Initialize struct to 0
// 	ft_exit(&data, args);
// 	printf("Exit status: %d | Exit flag: %d\n\n", data.ret, data.exit);
// }

// int main(void)
// {
// 	printf("=== TEST 1: Exiting with no arguments ===\n");
// 	char *args1[] = {"exit", NULL};
// 	test_exit(args1);

// 	printf("=== TEST 2: Exiting with a valid numeric argument ===\n");
// 	char *args2[] = {"exit", "42", NULL};
// 	test_exit(args2);

// 	printf("=== TEST 3: Exiting with a non-numeric argument ===\n");
// 	char *args3[] = {"exit", "hello", NULL};
// 	test_exit(args3);

// 	printf("=== TEST 4: Exiting with too many arguments ===\n");
// 	char *args4[] = {"exit", "123", "456", NULL};
// 	test_exit(args4);

// 	return 0;
// }
