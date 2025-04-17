/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:05:56 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 22:14:37 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_node(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->cmd)
		free(node->cmd);
	if (node->heredoc_delim)
		free(node->heredoc_delim);
	if (node->redir_file)
		free(node->redir_file);
	i = 0;
	if (node->arg)
	{
		while (node->arg[i])
			free(node->arg[i++]);
		free(node->arg);
		node->arg = NULL;
	}
	free(node);
	node = NULL;
}

void	free_nodes_list(t_node *head)
{
	t_node	*current;
	t_node	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_node(current);
		current = next;
	}
}
