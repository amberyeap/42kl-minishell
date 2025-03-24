/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:42:41 by ayeap             #+#    #+#             */
/*   Updated: 2024/07/04 17:12:03 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_print_lower_hex(int fd, unsigned int n, char *base)
{
	return (ft_print_digit(fd, n, base));
}

int	ft_print_upper_hex(int fd, unsigned int n, char *base)
{
	return (ft_print_digit(fd, n, base));
}
