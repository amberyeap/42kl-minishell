/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:06:51 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:25:48 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The memmove() function copies len bytes from string src to string dst. 
The two strings may overlap; the copy is 
always done in a non-destructive manner.

RETURN VALUES
The memmove() function returns the original value of dst. */

// #include <stdio.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	char	*dest2;
	char	*src2;

	dest2 = (char *)dest;
	src2 = (char *)src;
	if (dest2 == NULL && src2 == NULL)
		return (NULL);
	if (dest <= src)
	{
		while (len--)
			*dest2++ = *src2++;
	}
	else if (dest > src)
	{
		dest2 += len - 1;
		src2 += len - 1;
		while (len--)
			*dest2-- = *src2--;
	}
	return (dest);
}

/* int	main(void)
{
    char src[] = "hello world";
    char dest[50];
    int len = 6;
    ft_memmove(dest, src, len);
    printf("%s", dest);
    return (0);
} */