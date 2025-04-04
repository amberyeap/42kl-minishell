/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:19:38 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/04 19:21:00 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	sort_print_env(t_minishell *data)
{
	char **env_array;

	env_array = list_to_str(data->env);
	if (!env_array)
		return;
	sorting_env(env_array);
	print_sort(env_array);
	free_array(env_array);
}
// free env_node
static void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
// helper to create node for env list
t_env	*create_env(char *env_str)
{
	t_env	*node;// new node
	char	*equal;// to hold "="

	if (!env_str)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal = ft_strchr(env_str, '=');
	if (!equal)
	{
		free(node);
		return (NULL);
	}
	node->key = ft_strndup(env_str, (equal - env_str));
	node->value = ft_strdup(equal + 1);
	node->next = NULL;
	return (node);
}
/*
	* This function is to initialize the environment variable
	* it goes check if the envp passed is not null(nothing)
	* it create a first node and check if the node is NULL
	* then it goes continue on creating new node for the env list
	* of during the process node creation fails it should free the node
	* then it should link the new node with previous, move env pointer

*/
int	init_env(t_minishell *data, char **envp)
{
	t_env	*env;
	t_env	*new;
	int		i;

	i = 0;
	if (!envp || !envp[0])
		return (1);
	env = create_env(envp[i]);
	if (!env)
		return (1);
	data->env = env;
	while (envp[++i])
	{
		new = create_env(envp[i]);
		if (!new)
		{
			free_env(data->env);
			return (1);
		}
		env->next = new;//link new node to the previous node
		env = new;// move pointer to the new node
	}
	return (0);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include "../../minishell.h"  // Adjust path if needed

// // Function to print environment list (for testing purposes)
// void	print_env(t_env *env)
// {
// 	while (env)
// 	{
// 		printf("Key: %s, Value: %s\n", env->key, env->value);
// 		env = env->next;
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_minishell	data;

// 	(void)argc;
// 	(void)argv;

// 	// Test Case 1: Initialize environment with real envp
// 	printf("---- Test Case 1: Initializing environment ----\n");
// 	if (init_env(&data, envp) != 0)
// 	{
// 		printf("Error: Failed to initialize environment\n");
// 		return (1);
// 	}
// 	print_env(data.env);
// 	free_env(data.env);
// 	printf("\n");

// 	// Test Case 2: Initialize with an empty environment
// 	printf("---- Test Case 2: Initializing with empty environment ----\n");
// 	char *empty_env[] = {NULL};
// 	if (init_env(&data, empty_env) != 0)
// 		printf("Error: Failed to initialize empty environment (expected)\n");

// 	// Test Case 3: Initialize with a custom environment
// 	printf("---- Test Case 3: Initializing with a custom environment ----\n");
// 	char *custom_env[] = {
// 		"USER=testuser",
// 		"PATH=/usr/bin:/bin",
// 		"SHELL=/bin/bash",
// 		"HOME=/home/testuser",
// 		NULL
// 	};
// 	if (init_env(&data, custom_env) != 0)
// 	{
// 		printf("Error: Failed to initialize custom environment\n");
// 		return (1);
// 	}
// 	print_env(data.env);
// 	free_env(data.env);
// 	printf("\n");

// 	return (0);
// }
