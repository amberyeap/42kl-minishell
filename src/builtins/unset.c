/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:07:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/16 09:59:58 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Calculates the length of environment variable's key (before =)
static size_t	env_size(char *env)
{
	size_t	i;

	i = 0;
	if (!env)
		return (0);
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

// This tools helps for freeing the node that we want
static void	freenode(t_minishell *data, t_env *node)
{
	if (!data | !node)
		return ;
	if (node->value)
		free(node->value);
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
	int		len_name;

	// If no variable name is provided, return 0 for success
	if (!args[1])
		return (0);
	now_node = data->env;
	previous_node = NULL;
	while (now_node)
	{
		len_name = env_size(now_node->key);
		if (len_name > 0 && strncmp(args[1], now_node->key, len_name) == 0
			&& args[1][len_name] == '\0')
		{
			remove_env(data, previous_node, now_node);
			return (0);
		}
		previous_node = now_node;
		now_node = now_node->next;
	}
	return (0);
}
