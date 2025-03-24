/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:39:46 by muhabin-          #+#    #+#             */
/*   Updated: 2025/03/16 11:51:40 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = malloc(sizeof(n + 1));
	if (!dup)
		return (NULL);
	ft_strncpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}
