/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:15:05 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:44:06 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Parameters 
lst: The address of a pointer to a node.
f: The address of the function used to iterate on the list.

Description 
Iterates the list ’lst’ and applies the function
’f’ on the content of each node. */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}
