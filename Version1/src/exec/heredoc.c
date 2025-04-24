/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:06:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/22 16:25:01 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


int	is_directory(const char *path)
{
	int	fd;
	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0); //not a directory
}
int	quote_delim(char *delim)
{
	int	len;

	len = ft_strlen(delim);
	if ((delim[0] == '"' && delim[len - 1] == '"')
		|| (delim[0] == '\'' && delim[len - 1] == '\''))
		return (1);
	return (0);
}

/*
	* check the delimeter does it has quote or not
		#if NOT ("") it should not expand
	* then create pipe, receive input with while loop
	* check the line if the same as the delimeter
		#if 0 will break to print back what has been input
	* check whether it is inside a quotes or not
		#if not in quote it should expand the variable
		#then write all the input in the write-end pipe
		#then free the line
*/
int	setup_heredoc(t_node *node, t_minishell *data)
{
	int		pipefd[2];
	int		quoted;
	char	*line;
	char	*expanded;

	quoted = quote_delim(node->heredoc_delim);
	pipe(pipefd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, node->heredoc_delim) == 0)
			break ;
		if (!quoted) //unquoted delimiter
		{
			expanded = expand_var(line, data->exit);
			write(pipefd[1], expanded, ft_strlen(expanded));
			free(expanded);
		}
		else //quoted delimiter
			write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
