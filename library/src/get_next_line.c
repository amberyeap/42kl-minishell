/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:22:22 by ayeap             #+#    #+#             */
/*   Updated: 2024/07/25 14:53:00 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*new_stash(char *stash)
{
	char	*line;
	int		i;

	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	line = ft_strdup(stash + i + 1);
	free(stash);
	return (line);
}

char	*get_line(char *stash)
{
	char	*line;
	int		i;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	line = ft_substr(stash, 0, i + 1);
	return (line);
}

char	*read_fd(int fd, char *stash)
{
	char	*buffer;
	int		byte_read;
	char	*temp;

	if (!stash)
		stash = ft_strdup("");
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	byte_read = 1;
	while (byte_read > 0 && (!ft_strchr(buffer, '\n')))
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
			return (free(buffer), NULL);
		buffer[byte_read] = '\0';
		temp = ft_strjoin(stash, buffer);
		free(stash);
		stash = temp;
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	stash = read_fd(fd, stash);
	if (stash == NULL)
		return (NULL);
	line = get_line(stash);
	stash = new_stash(stash);
	return (line);
}
