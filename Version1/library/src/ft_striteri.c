/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 13:31:01 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:15:42 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION 
Applies the function ’f’ on each character of
the string passed as argument, passing its index
as first argument. Each character is passed by
address to ’f’ to be modified if necessary.

PARAMETERS 
s: The string on which to iterate.
f: The function to apply to each character.

RETURN VALUE
None. */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;
	unsigned int	length;

	if (s == NULL || f == NULL)
		return ;
	i = 0;
	length = ft_strlen(s);
	while (i < length)
	{
		f(i, &s[i]);
		i++;
	}
}
