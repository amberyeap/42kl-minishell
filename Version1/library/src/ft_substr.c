/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:57:30 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:07:40 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
DESCRIPTION
Allocates (with malloc(3)) and returns a substring from the string ’s’.
The substring begins at index ’start’ and is of maximum size ’len’.

PARAMETERS
s: The string from which to create the substring.
start: The start index of the substring in the string ’s’.
len: The maximum length of the substring.

RETURN VALUE
The substring. NULL if the allocation fails.
*/

// #include <stdio.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*subs;
	unsigned int	i;
	unsigned int	slen;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	if (len > slen)
		len = slen;
	subs = (char *)malloc(sizeof(char) * (len + 1));
	if (subs == NULL)
		return (NULL);
	if (start >= slen)
	{
		subs[0] = '\0';
		return (subs);
	}
	i = 0;
	while (i < len)
	{
		subs[i] = s[start + i];
		i++;
	}
	subs[i] = '\0';
	return (subs);
}

// int	main(void)
// {
// 	char *s = "hello world";
// 	char *result = ft_substr(s, 6, 5);
// 	printf("%s\n", result);
// 	return (0);
// }