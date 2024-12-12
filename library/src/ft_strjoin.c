/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:31:27 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/24 12:17:21 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
Allocates (with malloc(3)) and returns a new string, which is the 
result of the concatenation of ’s1’ and ’s2’.

PARAMETERS
s1: The prefix string.
s2: The suffix string.

RETURN VALUE
The new string. NULL if the allocation fails. */

// #include <stdio.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*s3;
	size_t	len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	s3 = malloc(sizeof(char) * (len + 1));
	if (s3 == NULL)
		return (NULL);
	i = 0;
	while (*s1 != '\0')
		s3[i++] = *s1++;
	while (*s2 != '\0')
		s3[i++] = *s2++;
	s3[i] = '\0';
	return (s3);
}

/* int	main(void)
{
	const char	*s1 = "Hello, ";
	const char	*s2 = "world!";

	char *result = ft_strjoin(s1, s2);
	printf("%s\n", result);
	return (0);
} */
