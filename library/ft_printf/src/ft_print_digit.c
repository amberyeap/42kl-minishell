/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_digit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:45:51 by ayeap             #+#    #+#             */
/*   Updated: 2024/07/04 17:12:01 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_printf.h"

int	ft_print_digit(long n, char *base)
{
	int	count;
	int	base_len;

	base_len = ft_strlen(base);
	if (n < 0)
	{
		write(1, "-", 1);
		return (ft_print_digit(-n, base) + 1);
	}
	else if (n < base_len)
		return (ft_print_char(base[n]));
	else
	{
		count = ft_print_digit(n / base_len, base);
		return (count + ft_print_digit(n % base_len, base));
	}
}
