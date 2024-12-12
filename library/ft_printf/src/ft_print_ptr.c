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

#include "../inc/ft_printf.h"

int	print_digit(unsigned long long n, char *base);

int	ft_print_ptr(void *ptr, char *base)
{
	write(1, "0x", 2);
	return (print_digit((unsigned long long)ptr, base) + 2);
}

int	print_digit(unsigned long long n, char *base)
{
	int					count;
	unsigned long long	base_len;

	base_len = ft_strlen(base);
	if (n < base_len)
		return (ft_print_char(base[n]));
	else
	{
		count = ft_print_digit(n / base_len, base);
		return (count + ft_print_digit(n % base_len, base));
	}
}
