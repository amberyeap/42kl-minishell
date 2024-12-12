/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 13:01:09 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:42:13 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Parameters 
lst: The address of a pointer to a node.
del: The address of the function used to delete the content of the node.

Return value 
None

Description 
Deletes and frees the given node and every
successor of that node, using the function ’del’ and free(3).
Finally, the pointer to the list must be set to NULL. */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst != NULL && del != NULL)
	{
		while (*lst != NULL)
		{
			temp = *lst;
			del((*lst)->content);
			*lst = (*lst)->next;
			free(temp);
		}
		*lst = NULL;
	}
}
