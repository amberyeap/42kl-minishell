/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:59:04 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:25:29 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The memcpy() function copies n bytes from memory area src to memory area dst.

RETURN VALUES
The memcpy() function returns the original value of dst. */

// #include <stdio.h>
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const char		*src2;
	char			*dest2;
	unsigned int	i;

	src2 = (char *)src;
	dest2 = (char *)dest;
	if (dest2 == NULL && src2 == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest2[i] = src2[i];
		i++;
	}
	return (dest2);
}

/* int main(void)
{
    char src[] = "hello world";
    char dest[50];
    int n = 5;
    ft_memcpy(dest, src, n);
    dest[n] = '\0';
    printf("%s", dest);
    return (0);
} */