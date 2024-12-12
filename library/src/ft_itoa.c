/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:05:43 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/15 15:23:26 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION 
Allocates (with malloc(3)) and returns a string
representing the integer received as an argument.
Negative numbers must be handled.

PARAMETERS 
n: the integer to convert.

RETURN VALUE
The string representing the integer.
NULL if the allocation fails. */

// #include <stdio.h>
#include "libft.h"

static size_t	int_length(int n);

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;

	nb = n;
	len = int_length(nb);
	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	if (nb == 0)
		*str = '0';
	if (nb < 0)
	{
		*str = '-';
		nb = -nb;
	}
	str[len] = '\0';
	while (nb > 0)
	{
		str[len - 1] = (nb % 10) + '0';
		nb /= 10;
		len--;
	}
	return (str);
}

static size_t	int_length(int n)
{
	size_t	length;

	length = 0;
	if (n <= 0)
		length++;
	while (n != 0)
	{
		n /= 10;
		length++;
	}
	return (length);
}

/* int	main(void)
{
	int n = -2147483648;
	char *str = ft_itoa(n);
	printf("%s", str);
	return (0);
}
 */