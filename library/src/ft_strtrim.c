/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:54:21 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:10:09 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION 
Allocates (with malloc(3)) and returns a copy of
’s1’ with the characters specified in ’set’ removed
from the beginning and the end of the string.

PARAMETERS
s1: The string to be trimmed.
set: The reference set of characters to trim.

RETURN VALUE
The trimmed string.
NULL if the allocation fails. */

// #include <stdio.h>
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s2;
	int		start;
	int		end;
	int		i;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] != '\0' && (ft_strchr(set, s1[start])))
		start++;
	end = ft_strlen(s1);
	while (end > start && (ft_strchr(set, s1[end - 1])))
		end--;
	s2 = (char *)malloc(sizeof(char) * (end - start + 1));
	if (s2 == NULL)
		return (NULL);
	i = 0;
	while (i < (end - start))
	{
		s2[i] = s1[start + i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

// int main(void)
// {
//     char *s1 = "  hello world  ";
// 	char *set = " ";
// 	char *result = ft_strtrim(s1, set);
// 	printf("%s", result);
// 	return (0);
// }
