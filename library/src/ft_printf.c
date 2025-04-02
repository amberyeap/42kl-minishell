/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:19:52 by ayeap             #+#    #+#             */
/*   Updated: 2024/07/17 12:33:42 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int	print_format(int fd, char c, va_list ap)
{
	char	*base10;
	char	*lower_base16;
	char	*upper_base16;

	base10 = "0123456789";
	lower_base16 = "0123456789abcdef";
	upper_base16 = "0123456789ABCDEF";
	if (c == 'c')
		return (ft_print_char(fd, va_arg(ap, int)));
	else if (c == 's')
		return (ft_print_str(fd, va_arg(ap, char *)));
	else if (c == 'p')
		return (ft_print_ptr(fd, va_arg(ap, void *), lower_base16));
	else if (c == 'd' || c == 'i')
		return (ft_print_digit(fd, va_arg(ap, int), base10));
	else if (c == 'u')
		return (ft_print_digit(fd, va_arg(ap, unsigned int), base10));
	else if (c == 'x')
		return (ft_print_lower_hex(fd, va_arg(ap, unsigned int), lower_base16));
	else if (c == 'X')
		return (ft_print_upper_hex(fd, va_arg(ap, unsigned int), upper_base16));
	else
		return (write(fd, &c, 1));
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		count;

	if (format == NULL)
		return (1);
	va_start(ap, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			count += print_format(fd, *(++format), ap);
		else
			count += write(fd, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}
