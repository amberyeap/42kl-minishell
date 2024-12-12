/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:53:32 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:32:16 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
DESCRIPTION
The tolower() function converts an upper-case letter 
to the corresponding lower-case letter.
The argument must be representable as an unsigned char or the value of EOF.
*/

// #include <stdio.h>

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

/* int main(void)
{
    char c = 'G';
    printf("%c\n", ft_tolower(c));
    return (0);
} */