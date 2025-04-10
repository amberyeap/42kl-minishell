/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:00:15 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/10 16:11:59 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	apply_n_close(int fd, int std_fd)
{
	if (fd != -1)
	{
		dup2(fd, std_fd);
		close(fd);
	}
}

int	handle_out(t_node *node, int *fd_out)
{
	if (*fd_out != -1)
		close(*fd_out);
	if (node->type == NODE_REDIR_OUT)
		*fd_out = open(node->redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->type == NODE_APPEND)
		*fd_out = open(node->redir_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd_out == -1)
	{
		perror("Redirection Erorr");
		return (-1);
	}
	return (0);
}

int	handle_in(t_node *node, int *fd_in)
{
	if (*fd_in != -1) //Closed previous redir input if exist before
		close(*fd_in);
	if (node->type == NODE_REDIR_IN) //"<" cases
		*fd_in = open(node->redir_file, O_RDONLY);
	else if (node->type == NODE_HEREDOC) //"<<" Cases
		*fd_in = setup_heredoc(node);
	if (*fd_in == -1)
	{
		perror("Redirection Error");
		return (-1);
	}
	return (0);
}

//Check the Redirection Node
int	check_redir(t_node *node)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	while (node)
	{
		if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
		{
			if (handle_in(node, &fd_in) == -1)
				return (-1);
		}
		else if (node->type == NODE_REDIR_OUT || node->type == NODE_APPEND)
		{
			if (handle_out(node, &fd_out) == -1)
				return (-1);
		}
		node = node->next;
	}
	apply_n_close(fd_in, STDIN_FILENO);
	apply_n_close(fd_out, STDOUT_FILENO);
	return (0);
}
