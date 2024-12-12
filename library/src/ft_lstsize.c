/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 09:30:05 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:20:00 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Description 
Counts the number of nodes in a list.

Parameters 
lst: The beginning of the list.

Return value 
The length of the list */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst -> next;
	}
	return (count);
}
