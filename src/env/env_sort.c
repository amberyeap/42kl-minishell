/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:31:10 by muhabin-          #+#    #+#             */
/*   Updated: 2025/03/21 17:30:50 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_sort(char **sorted_env)
{
	int i;
	char	*equal;
	i = 0;
	while (sorted_env[i])
	{
		equal = ft_strchr(sorted_env[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n",sorted_env[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n",sorted_env[i]);
		i++;
	}
}
void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}
//count node
static int	count_node(t_env *env)
{
	int	i;

	i = 0;
	while (env != NULL)
	{
		env = env->next;
		i++;
	}
	return (i);
}

/*
	* How it works converting list node to array?
		#need to count the list node first
		#malloc array then + 1 for NULL
		#join the key with "=" in the array
			array[0] = PATH=
		#join the value later
			array[0] = PATH=/usr/bin
*/
char	**list_to_str(t_env *env)
{
	int	count;
	char **array;
	int	i;

	count = count_node(env);
	array = malloc(sizeof(char *) * (count + 1));
	if (!env || !array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i] = ft_strjoin(env->key, "=");
		array[i] = ft_strjoin(array[i], env->value);
		if (!array[i])
		{
			free_array(array);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
// insertion sorting method
void	sorting_env(char **array_env)
{
	int		i;
	int		j;
	char	*temp;

	if (!array_env || !array_env[0])
		return;
	i = 0;
	while (array_env[i])
	{
		temp = array_env[i];
		j = i - 1;
		while (j >= 0 && ft_strcmp(array_env[j], temp) > 0)
		{
			array_env[j + 1] = array_env[j];
			j--;
		}
		array_env[j + 1] = temp;
		i++;
	}
}

// #include <stdio.h>
// #include <stdlib.h>
// #include "../../minishell.h"

// // Function to create a new environment node
// t_env	*create_env_node(char *key, char *value)
// {
// 	t_env	*node;

// 	node = malloc(sizeof(t_env));
// 	if (!node)
// 		return (NULL);
// 	node->key = ft_strdup(key);
// 	node->value = ft_strdup(value);
// 	node->next = NULL;
// 	return (node);
// }

// // Function to free the environment linked list
// void	free_env_list(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env;
// 		env = env->next;
// 		free(tmp->key);
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

// // Main function to test sort_print_env
// int	main(void)
// {
// 	t_minishell	data;

// 	// Initialize the environment linked list
// 	data.env = create_env_node("PATH", "/usr/bin");
// 	data.env->next = create_env_node("HOME", "/home/user");
// 	data.env->next->next = create_env_node("APPLE", "red");
// 	data.env->next->next->next = create_env_node("ZEBRA", "stripes");
// 	data.env->next->next->next->next = create_env_node("BANANA", "yellow");

// 	// Call the sort_print_env function
// 	sort_print_env(&data);

// 	// Free the environment linked list
// 	free_env_list(data.env);

// 	return (0);
// }
