/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:57:49 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:26:25 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The strchr() function locates the first occurrence of c 
(converted to a char) in the string pointed to by s. 
The terminating null character is considered to be part of the string;
therefore if c is `\0', the functions locate the terminating `\0'.

The strrchr() function is identical to strchr(), except it 
locates the last occurrence of c.

RETURN VALUES
The functions strchr() and strrchr() return a pointer to the 
located character, or NULL if the character does not appear in the string. */

// #include <stdio.h>
#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str != '\0')
	{
		if ((unsigned char)*str == (unsigned char)c)
			return ((char *)str);
		str++;
	}
	if (c == '\0')
		return ((char *)str);
	return (NULL);
}

/* int main(void)
{
    char *str = "hello world";
    char c = 'l';
    printf("after %c is %s", c, ft_strchr(str, c));
    return (0);
} */