/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:07:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/27 22:54:16 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// This tools helps for freeing the node that we want
static void	freenode(t_minishell *data, t_env *node)
{
	if (!data || !node)
		return ;
	if (node->key)
		free(node->key);
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
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		now_node = data->env;
		previous_node = NULL;
		while (now_node)
		{
			if (ft_strcmp(args[i], now_node->key) == 0)
			{
				remove_env(data, previous_node, now_node);
				break ;
			}
			previous_node = now_node;
			now_node = now_node->next;
		}
		i++;
	}
	return (0);
}
