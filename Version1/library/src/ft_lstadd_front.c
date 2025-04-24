/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 09:26:04 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:33:35 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Description 
Adds the node ’new’ at the beginning of the list.

Parameters 
lst: The address of a pointer to the first link of a list.
new: The address of a pointer to the node to be added to the list.
*/

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}
