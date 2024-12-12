/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 13:51:14 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:17:12 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Description 
Outputs the integer ’n’ to the given file descriptor. */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	num;
	char	digit;

	num = n;
	if (num < 0)
	{
		write (fd, "-", 1);
		num = -num;
	}
	if (num >= 10)
		ft_putnbr_fd(num / 10, fd);
	digit = num % 10 + '0';
	write (fd, &digit, 1);
}
