/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:21:52 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:21:25 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The isalpha() function tests for any character for 
which isupper(3) or islower(3) is true.
The value of the argument must be representable 
as an unsigned char or the value of EOF.

RETURN VALUES
The isalpha() function returns zero if the character 
tests false and returns non-zero if the
character tests true. */

// #include <stdio.h>

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

/* int main(void)
{
    char c = 'a';
    printf("%d", ft_isalpha(c));
    return (0);
} */