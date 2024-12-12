/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 13:05:48 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/19 12:27:08 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);
int		ft_print_char(char c);
int		ft_print_str(char *str);
int		ft_print_digit(long n, char *base);
int		ft_print_lower_hex(unsigned int n, char *base);
int		ft_print_upper_hex(unsigned int n, char *base);
int		ft_print_ptr(void *ptr, char *base);

int		ft_strlen(const char *str);

#endif