/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:19:54 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:23:35 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The memchr() function locates the first 
occurrence of c (converted to an unsigned char) in
string s.

RETURN VALUES
The memchr() function returns a pointer to the 
byte located, or NULL if no such byte exists
within n bytes. */

/* #include <stdio.h>
#include <string.h> */
#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*str2;

	str2 = (unsigned char *)str;
	i = 0;
	while (i < n)
	{
		if (str2[i] == (unsigned char)c)
			return ((void *)&str2[i]);
		i++;
	}
	return (NULL);
}

/* int main(void)
{
    char *str = "hello world";
    char c = 'l';
    unsigned int n = strlen(str);
    printf("after %c is %s", c, ft_memchr(str, c, n));
    return (0);
} */