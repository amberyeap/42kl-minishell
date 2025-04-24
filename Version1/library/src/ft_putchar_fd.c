/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 13:29:34 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:16:07 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Description 
Outputs the character ’c’ to the given file descriptor. */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}
