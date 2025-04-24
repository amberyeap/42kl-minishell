/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:14:06 by ayeap             #+#    #+#             */
/*   Updated: 2024/06/13 13:13:09 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* DESCRIPTION 
Allocates (with malloc(3)) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter. The array must end
with a NULL pointer.

PARAMETERS
s: The string to be split.
c: The delimiter character.

RETURN VALUE
The array of new strings resulting from the split.
NULL if the allocation fails. */

#include "libft.h"

static char	**split_str(char const *s, char c, int total_words);
static int	count_words(char const *s, char c);
static char	*copy_word(char const *s, int len);
static void	free_memory(char **result, int words);

char	**ft_split(char const *s, char c)
{
	char		**result;
	int			total_words;

	if (s == NULL)
		return (NULL);
	total_words = count_words(s, c);
	result = split_str(s, c, total_words);
	return (result);
}

static char	**split_str(char const *s, char c, int total_words)
{
	char		**result;
	const char	*start;
	int			i;

	result = (char **)malloc(sizeof(char *) * (total_words + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < total_words)
	{
		while (*s == c)
			s++;
		start = s;
		while (*s && *s != c)
			s++;
		result[i] = copy_word(start, s - start);
		if (result[i] == NULL)
		{
			free_memory(result, i);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

static int	count_words(char const *s, char c)
{
	int	word;

	word = 0;
	while (*s != '\0')
	{
		if (*s == c)
			s++;
		else if (*s != c)
		{
			word++;
			while (*s && *s != c)
				s++;
		}
	}
	return (word);
}

static char	*copy_word(char const *s, int len)
{
	char	*str;
	int		i;

	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[len] = '\0';
	return (str);
}

static void	free_memory(char **result, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(result[i]);
		i++;
	}
	free(result);
}
