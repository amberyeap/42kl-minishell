/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:00:15 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:27:56 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
strlcpy() copies up to dstsize - 1 characters from the 
string src to dst, NUL-terminating
the result if dstsize is not 0.

RETURN VALUE
strlcpy() function returns the total length of the
string they tried to create.  For strlcpy() that means the length of src. */

// #include <stdio.h>
#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	unsigned int	i;
	unsigned int	srclen;

	srclen = ft_strlen(src);
	if (destsize == 0)
		return (srclen);
	i = 0;
	while ((i < destsize - 1) && (src[i] != '\0'))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (srclen);
}

/* int main(void)
{
    const char src[] = "hello world";
    char dest[50];
    printf("%zu\n", ft_strlcpy(dest, src, 50));
    printf("%s", dest);
    return (0);
} */