/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 19:20:23 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/14 19:58:01 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The calloc() function contiguously allocates enough
space for count objects that are size bytes of memory
each and returns a pointer to the allocated memory.
The allocated memory is filled with bytes of value
zero. */

// count = number of data in array, size = bytes of EACH data
/* size * count > UINT_MAX 
size * count / size > UINT_MAX / size
count > UINT_MAX / size */

#include "libft.h"
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>

void	*ft_calloc(size_t count, size_t size);
// {
// 	char	*ptr;
// 	size_t	totalb;

// 	if (size != 0 && count > UINT_MAX / size)
// 		return (NULL);
// 	totalb = count * size;
// 	ptr = malloc(totalb);
// 	if (ptr == NULL)
// 		return (NULL);
// 	ft_memset(ptr, 0, totalb);
// 	return (ptr);
// }

// int main(void)
// {
// 	int n = 10;
// 	int *str = ft_calloc(n, sizeof(int));

// 	int i = 0;
// 	while (i < n)
// 	{
// 		printf("%d", str[i]);
// 		i++;

// 	printf("\n");
// 	}
// }

void	*ft_calloc(size_t count, size_t size)
{
	char	*ptr;
	size_t	totalb;
	size_t	i;

	if (size != 0 && count > UINT_MAX / size)
		return (NULL);
	totalb = count * size;
	ptr = malloc(totalb);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < totalb)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}
