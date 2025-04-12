/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 08:01:54 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/12 17:25:28 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * @brief this exit the shell
 * @note what should it handle?
 * @note need to specify...
*/
int	ft_exit(t_minishell *data, char **args)
{
	data->exit = 1;// set exit to 1 first
	ft_putstr_fd("exit ", STDERR_FILENO);
	// handle with to many arguments
	if (args[0] && args[1])
	{
		data->exit = 1;// set the return value to 1(error)
		ft_fprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (1);
	}
	//Handle for non-numeric arguments
	else if (args[0] && !ft_strisnum(args[0]))
	{
		// data->ret = 255;
		ft_fprintf(STDERR_FILENO, "minishell: exit: %s:numeric argument required\n", args[0]);
		exit (255);
	}
	//handle valid numeric arguments
	else if (args[0])
		// data->ret = ft_atoi(args[1]);
		exit(ft_atoi(args[0]));
	else// handle no arguments
		// data->ret = 0;
		exit (0);
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
