/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:24:28 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:22:53 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The isdigit() function tests for a 
decimal digit character.  
Regardless of locale, this includes the following 
characters only from '0' to '9'.
The value of the argument must be representable as an 
unsigned char or the value of EOF.

RETURN VALUES
The isdigit() and isnumber() functions 
return zero if the character tests false 
and return non-zero if the character tests true. */

// #include <stdio.h>

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

/* int main(void)
{
    int c = 'a';
    printf("%d", ft_isdigit(c));
    return (0);
} */