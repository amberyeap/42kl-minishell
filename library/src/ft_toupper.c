/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:48:08 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:32:10 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The toupper() function converts a lower-case letter 
to the corresponding upper-case letter.
The argument must be representable as an unsigned char or the value of EOF.
*/

// #include <stdio.h>

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

/* int main(void)
{
    char c = 'g';
    printf("%c\n", ft_toupper(c));
    return (0);
} */