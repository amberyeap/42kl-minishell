/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:06:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:32:25 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*movin_node(t_node *node, int has_pipe)
{
	if (has_pipe)
		return (node->next->next);
	return (node->next);
}

int	is_directory(const char *path)
{
	int	fd;

	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}

int	quote_delim(t_node *node)
{
	if (node->quoted == true)
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

	quoted = quote_delim(node);
	pipe(pipefd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, node->heredoc_delim) == 0)
			break ;
		if (!quoted)
		{
			expanded = expand_var(line, data->exit, data);
			write(pipefd[1], expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
