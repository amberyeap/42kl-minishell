/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 02:07:37 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:48:12 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	input_redir(t_node *node, t_minishell *shell)
{
	int	fd;

	fd = open(node->redir_file, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			node->redir_file, strerror(errno));
		shell->exit = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	output_redir(t_node *node, t_minishell *shell)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (node->type == NODE_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(node->redir_file, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			node->redir_file, strerror(errno));
		shell->exit = 1;
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc(t_node *node, t_minishell *shell)
{
	int	fd;

	fd = setup_heredoc(node, shell);
	if (fd == -1)
	{
		shell->exit = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

void	print_and_exit_error(char *prefix, char *arg, int code)
{
	ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n", arg, prefix);
	exit(code);
}
