/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:40:02 by ayeap             #+#    #+#             */
/*   Updated: 2024/09/09 10:32:49 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The strdup() function allocates sufficient memory 
for a copy of the string s1, does the
copy, and returns a pointer to it.  The pointer 
may subsequently be used as an argument to
the function free(3).

If insufficient memory is available, 
NULL is returned and errno is set to ENOMEM. */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

/* static char	*ft_strcpy(char *dest, const char *src);

char	*ft_strdup(const char *s1)
{
	char	*dest;

	dest = (char *)malloc((ft_strlen(s1) + 1) * (sizeof(char)));
	if (dest == NULL)
		return (NULL);
	ft_strcpy(dest, s1);
	return (dest);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
} */

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/* int	main(void)
{
	char	*str1;
	char	*str2;

	str1 = "hello world";
	str2 = ft_strdup(str1);
	printf("%s\n", str2);
	return (0);
} */
