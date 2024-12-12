/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 09:35:15 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:25:08 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The memcmp() function compares byte string s1 against byte string s2.  
Both strings are assumed to be n bytes long. */

// #include <stdio.h>
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

/* int main(void)
{
    char *s1 = "hello world";
    char *s2 = "helloo world";
    unsigned int n = 7;

    if (ft_memcmp(s1, s2, n) == 0)
        printf("same");
    else
        printf("different");
    return (0);
} */