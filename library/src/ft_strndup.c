/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:39:46 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 20:03:19 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = ft_calloc(n + 1, n);
	if (!dup)
		return (NULL);
	ft_strncpy(dup, s, n);
	return (dup);
}
