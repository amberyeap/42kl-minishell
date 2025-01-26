/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:28:41 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/14 20:00:36 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
strlcat() appends string src to the end of dst.  
It will append at most dstsize - strlen(dst) - 1 characters.  
It will then NUL-terminate, unless dstsize is 0 or the original
dst string was longer than dstsize

If the src and dst strings overlap, the behavior is undefined.

RETURN VALUE
return the total length of the string they tried to create. For strlcat()
that means the initial length of dst plus the length of src. */

// #include <stdio.h>
#include "libft.h"

// int ft_strlen(const char *str)
// {
// 	int len = 0;
// 	while (str[len])
// 		len++;
// 	return (len);
// }

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;

	srclen = ft_strlen(src);
	if (dst == NULL && dstsize == 0)
		return (srclen);
	dstlen = ft_strlen(dst);
	if (dstsize <= dstlen)
		return (dstsize + srclen);
	i = 0;
	while (src[i] && (dstlen + i) < dstsize - 1)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}

// int main(void)
// {
//     const char src[] = "hello world";
//     char dest[] = "morning, ";
//     printf("%zu\n", ft_strlcat(dest, src, 20));
//     printf("%s\n", dest);
//     return (0);
// }