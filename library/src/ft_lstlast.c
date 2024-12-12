/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 09:41:55 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:32:51 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Parameters 
lst: The beginning of the list.

Return value 
Last node of the list

Description 
Returns the last node of the list. */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}
