/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 08:01:54 by muhabin-          #+#    #+#             */
/*   Updated: 2025/02/25 09:14:10 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * @brief this exit the shell
 * @note what should it handle?
 * @note need to specify...
*/
void	ft_exit(t_minishell *data, char **args)
{
	data->exit = 1;// set exit to 1 first
	ft_putstr_fd("exit ", STDERR_FILENO);
	// handle with to many arguments
	if (args[1] && args[2])
	{
		data->ret = 1;// set the return value to 1(error)
		ft_putendl_fd("minishell: exit: too many arguments",STDERR_FILENO);
	}
	//Handle for non-numeric arguments
	else if (args[1] && ft_strisnum(args[1]) == 0)
	{
		data->ret = 255;
		ft_putstr_fd("minishell: exit: ",STDERR_FILENO);
		ft_putstr_fd(args[1],STDERR_FILENO);
		ft_putendl_fd(":numeric argument required", STDERR_FILENO);
	}
	//handle valid numeric arguments
	else if (args[1])
		data->ret = ft_atoi(args[1]);
	else// handle no arguments
		data->ret = 0;
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
