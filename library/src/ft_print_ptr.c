/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:37:15 by ayeap             #+#    #+#             */
/*   Updated: 2024/07/04 17:12:05 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	print_digit(int fd, unsigned long long n, char *base);

int	ft_print_ptr(int fd, void *ptr, char *base)
{
	write(fd, "0x", 2);
	return (print_digit(fd, (unsigned long long)ptr, base) + 2);
}

int	print_digit(int fd, unsigned long long n, char *base)
{
	int					count;
	unsigned long long	base_len;

	base_len = ft_strlen(base);
	if (n < base_len)
		return (ft_print_char(fd, base[n]));
	else
	{
		count = ft_print_digit(fd, n / base_len, base);
		return (count + ft_print_digit(fd, n % base_len, base));
	}
}
