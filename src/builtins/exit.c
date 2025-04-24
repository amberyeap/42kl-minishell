/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 08:01:54 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/18 07:48:00 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_symbol(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
			return (1);
		else if (str[i] == '+')
			return (2);
		i++;
	}
	return (0);
}

/*
1. handles too many arguments
2. handles integer with '-' symbol
3. handles integer with '+' symbol
4. handles non-numeric arguments
5. handles valid numeric arguments (without '-' or '+')
6. handles no arguments
*/
int	ft_exit(t_minishell *data, char **args)
{
	data->exit = 1;
	ft_fprintf(STDERR_FILENO, "exit\n");
	if (length(args) > 1 && args[1] && args[2])
	{
		data->exit = 1;
		ft_fprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (1);
	}
	else if (length(args) > 1 && check_symbol(args[1]) == 1)
		exit(256 + ft_atoi(args[1]));
	else if (length(args) > 1 && check_symbol(args[1]) == 2)
		exit(ft_atoi(args[1]));
	else if (length(args) > 1 && args[1] && !ft_strisnum(args[1]))
	{
		ft_fprintf(STDERR_FILENO, "minishell: exit: ");
		ft_fprintf(STDERR_FILENO, "%s: numeric argument required\n", args[1]);
		exit(2);
	}
	else if (length(args) > 1 && args[1])
		exit(ft_atoi(args[1]));
	else
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
