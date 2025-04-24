/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:39:28 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:28:19 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The strlen() function computes the length of the string s. 
The strnlen() function attempts to compute the length of s, 
but never scans beyond the first maxlen bytes of s.

RETURN VALUES
The strlen() function returns the number of characters 
that precede the terminating NUL
character.  The strnlen() function returns 
either the same result as strlen() or maxlen,
whichever is smaller. */

// #include <stdio.h>
#include "libft.h"
#include "strings.h"

int	ft_strlen(const char *str)
{
	int	length;

	length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	return (length);
}

// int main(void)
// {
//     char *str = NULL;
//     printf("%d", ft_strlen(str));

//     return (0);
// } 
