/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:24 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:21:15 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The isalnum() function tests for any character for 
which isalpha(3) or isdigit(3) is true.
The value of the argument must be representable 
as an unsigned char or the value of EOF.

RETURN VALUES
The isalnum() function returns zero if the character 
tests false and returns non-zero if the
character tests true. */

// #include <stdio.h>

int	ft_isalnum(int c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	else if (c == '_')
		return (1);
	else
		return (0);
}

/* int  main(void)
{
    int c = 'a';
    printf("%d", ft_isalnum(c));
    return (0);
} */