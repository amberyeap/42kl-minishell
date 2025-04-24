/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:56:26 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:23:18 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The isprint() function tests for any printing character, 
including space.  The value of the argument must be 
representable as an unsigned char or the value of EOF.

RETURN VALUES
The isprint() function returns zero if the character 
tests false and returns non-zero if the
character tests true. */

// #include <stdio.h>

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	else
		return (0);
}

/* int main(void)
{
    int c = 'n';
    printf("%d", ft_isprint(c));
    return (0);
} */