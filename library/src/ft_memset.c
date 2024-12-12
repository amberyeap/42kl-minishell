/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:08:00 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:26:04 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The memset() function writes len bytes of value c 
(converted to an unsigned char) to the string b.

RETURN VALUES
The memset() function returns its first argument. */

// #include <stdio.h>
#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned int	i;
	unsigned char	*str;

	str = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

/* int main(void)
{
    char str[] = "hello world";
    ft_memset(str, '.', 6);
    printf("%s", str);
    return (0);
} */