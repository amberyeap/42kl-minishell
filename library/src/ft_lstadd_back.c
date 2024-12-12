/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 09:49:03 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/14 20:00:52 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Parameters 
lst: The address of a pointer to the first link of a list.
new: The address of a pointer to the node to be
added to the list.

Description 
Adds the node ’new’ at the end of the list. */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last -> next != NULL)
		last = last -> next;
	last -> next = new;
}
