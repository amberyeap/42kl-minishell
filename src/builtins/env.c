/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:45:45 by muhabin-          #+#    #+#             */
/*   Updated: 2025/02/25 07:44:59 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_env(char **args, t_env *env)
{
	if (args[1])
	{
		ft_putstr_fd("env: No such file or directory",2);
		return (127);
	}
	if (!env)
		return (0);
	while (env)
	{
		ft_putendl_fd(env->value,1);
		env = env->next;
	}
	return (0);
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>


// // Helper function to create an environment list node
// t_env *create_env_node(char *value)
// {
// 	t_env *new_node = malloc(sizeof(t_env));
// 	if (!new_node)
// 		return (NULL);
// 	new_node->value = strdup(value); // Duplicate string for safety
// 	new_node->next = NULL;
// 	return (new_node);
// }

// // Helper function to free environment list
// void free_env_list(t_env *env)
// {
// 	t_env *temp;
// 	while (env)
// 	{
// 		temp = env;
// 		env = env->next;
// 		free(temp->value);
// 		free(temp);
// 	}
// }

// int main(void)
// {
// 	// ✅ TEST CASE 1: Valid environment list (should print variables)
// 	t_env *env = create_env_node("VAR1=value1");
// 	env->next = create_env_node("VAR2=value2");
// 	env->next->next = create_env_node("VAR3=value3");

// 	printf("\n=== TEST 1: Running `env` without arguments ===\n");
// 	char *args1[] = {"env", NULL};
// 	int result1 = ft_env(args1, env);
// 	printf("Return value: %d\n", result1);

// 	// ✅ TEST CASE 2: Extra argument (should print error and return 127)
// 	printf("\n=== TEST 2: Running `env` with extra arguments ===\n");
// 	char *args2[] = {"env", "EXTRA_ARG", NULL};
// 	int result2 = ft_env(args2, env);
// 	printf("Return value: %d\n", result2);

// 	// ✅ TEST CASE 3: Empty environment list (should return 0)
// 	printf("\n=== TEST 3: Running `env` with an empty environment ===\n");
// 	char *args3[] = {"env", NULL};
// 	int result3 = ft_env(args3, NULL);
// 	printf("Return value: %d\n", result3);

// 	// Free allocated memory
// 	free_env_list(env);
// 	return 0;
// }
