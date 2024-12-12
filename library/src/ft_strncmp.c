/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:49:49 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:30:08 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The strncmp() function compares not more than n characters. 
Characters that appear after a `\0' character are not compared.

RETURN VALUES
The strncmp() functions return an integer greater than, equal to, 
or less than 0, according as the string s1 is 
greater than, equal to, or less than the string s2.  
The comparison is done using unsigned characters, 
so that `\200' is greater than `\0'. */

// #include <stdio.h>
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n > 0 && *str1 && *str2)
	{
		if (*str1 != *str2)
			return ((int)*str1 - *str2);
		str1++;
		str2++;
		n--;
	}
	if (n == 0)
		return (0);
	else
		return ((int)*str1 - *str2);
}

/* int main(void)
{
    char *s1 = "hello world";
    char *s2 = "helloo world";
    unsigned int n = 7;

    if (ft_strncmp(s1, s2, n) == 0)
        printf("same");
    else
        printf("different");
    return (0);
} */