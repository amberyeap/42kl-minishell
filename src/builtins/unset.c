/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:07:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/04 14:08:56 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// Calculates the length of environment variable's key (before =)
static size_t	env_size(char *env)
{
	size_t	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

// This tools helps for freeing the node that we want
static void	freenode(t_minishell *data, t_env *node)
{
	if (!data | !node)//if the node is NULL or data not init, do nothin
		return;
	if (node->value)//check if theres is value inside
		free(node->value);// this free the value store
	// if the node to free is the head of the list, update the head
	if (data->env == node)
		data->env = node->next;
	free(node);
}
// This function removes the selected node by using freenode function
void	remove_env(t_minishell *data, t_env *previous_node, t_env *remove_node)
{
	if (!previous_node)
		data->env = remove_node->next;
	else
		previous_node->next = remove_node->next;
	freenode(data, remove_node);
}
/*
 * @brief unset command remove the environment variable from the shell's env
 * @param args only remove what comes after unset
*/
int	unset(char **args, t_minishell *data)
{
	t_env	*now_node;
	t_env	*previous_node;

	// If no variable name is provided, return 0 for success
	if (!args[1])
		return (1);
	now_node = data->env;// Start from the head of the environment list
	previous_node = NULL;// Initialize to Null

	// Traverse the environment list
	while (now_node)
	{
		// Check if the current node matches the variable to unset
		if (strncmp(args[1], now_node->value,
				env_size(now_node->value)) == 0)
		{
			remove_env(data, previous_node, now_node);//remove the matching node
			return (1);
		}
		// Move to the next node
		previous_node = now_node;
		now_node = now_node->next;
	}
	return (0);// Return success if no matching node was found
}

// // Function prototypes
// void    remove_env(t_minishell *data, t_env *previous_node, t_env *remove_node);
// int     unset(char **args, t_minishell *data);
// size_t  env_size(char *env);
// void    freenode(t_minishell *data, t_env *node);

// // Helper function to create an environment variable node
// t_env *create_env_node(const char *value)
// {
//     t_env *new_node = malloc(sizeof(t_env));
//     if (!new_node)
//         return NULL;
//     new_node->value = strdup(value);
//     new_node->next = NULL;
//     return new_node;
// }

// // Helper function to print the environment list
// void print_env_list(t_env *head)
// {
//     t_env *current = head;
//     while (current)
//     {
//         printf("%s\n", current->value);
//         current = current->next;
//     }
// }

// // Helper function to free the entire environment list
// void free_env_list(t_env *head)
// {
//     t_env *tmp;
//     while (head)
//     {
//         tmp = head;
//         head = head->next;
//         free(tmp->value);
//         free(tmp);
//     }
// }

// // **Test Case**
// int main(void)
// {
//     t_minishell mini;
//     char *args1[] = {"unset", "VAR2", NULL};
//     char *args2[] = {"unset", "VAR3", NULL};

//     // Setup environment list
//     mini.env = create_env_node("VAR1=value1");
//     mini.env->next = create_env_node("VAR2=value2");
//     mini.env->next->next = create_env_node("VAR3=value3");

//     printf("Before unsetting:\n");
//     print_env_list(mini.env);

//     // Test unsetting VAR2
//     unset(args1, &mini);
//     printf("\nAfter unsetting VAR2:\n");
//     print_env_list(mini.env);

//     // Test unsetting VAR3
//     unset(args2, &mini);
//     printf("\nAfter unsetting VAR3:\n");
//     print_env_list(mini.env);

//     // Free remaining environment list
//     free_env_list(mini.env);
//     return 0;
// }
