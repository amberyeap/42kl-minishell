/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:35:50 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:30:56 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION
The strnstr() function locates the first occurrence of the 
null-terminated string needle in the string haystack, 
where not more than len characters are searched.
Characters that appear after a `\0' character are not searched.  
Since the strnstr() function is a FreeBSD specific API, 
it should only be used when portability is not a concern.

RETURN VALUES
If needle is an empty string, haystack is returned; 
if needle occurs nowhere in haystack,
NULL is returned; otherwise a pointer to the 
first character of the first occurrence of
needle is returned. */

// #include <stdio.h>
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	n;
	size_t	i;

	if (!haystack && len == 0)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	n = 0;
	while (haystack[n] && n < len)
	{
		i = 0;
		while (haystack[n + i] && needle[i] && (haystack[n + i] == needle[i]
				&& i + n < len))
			i++;
		if (needle[i] == '\0')
			return ((char *)(haystack + n));
		n++;
	}
	return (NULL);
}

/* int main() {
    const char *haystack = "aaabcabcd";
    const char *needle = "aabc";
    // size_t len = -1;

    char *result = ft_strnstr(haystack, needle, -1);
    if (result) {
        printf("Found: '%s'\n", result);
    } else {
        printf("Not found\n");
    }

    return 0;
} */