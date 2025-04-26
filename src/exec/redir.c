/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:00:15 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:28:28 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	reset_fd(int ori_stdin, int ori_stdout, t_minishell *shell)
{
	dup2(ori_stdin, STDIN_FILENO);
	dup2(ori_stdout, STDOUT_FILENO);
	shell->exit = 1;
	return (-1);
}

static int	find_last_redir(t_node *node, t_redir *info, t_minishell *shell)
{
	t_node	*current;

	current = node;
	while (current)
	{
		if (current->type == NODE_HEREDOC)
			info->last_heredoc = current;
		else if (current->type == NODE_REDIR_IN)
		{
			if (access(current->redir_file, F_OK) == -1)
			{
				ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
					current->redir_file, strerror(errno));
				shell->exit = 1;
				return (-1);
			}
			info->last_in = current;
		}
		else if (current->type == NODE_REDIR_OUT
			|| current->type == NODE_APPEND)
			info->last_out = current;
		current = current->next;
	}
	return (0);
}

int	redir(t_redir *info, t_minishell *shell)
{
	int	result;

	result = 0;
	if (info->last_heredoc)
		result = handle_heredoc(info->last_heredoc, shell);
	if (result == 0 && info->last_in)
		result = input_redir(info->last_in, shell);
	if (result == 0 && info->last_out)
		result = output_redir(info->last_out, shell);
	return (result);
}

int	check_redir(t_node *node, t_minishell *shell)
{
	int				original_stdin;
	int				original_stdout;
	int				result;
	t_redir			info;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	info.last_in = NULL;
	info.last_out = NULL;
	info.last_heredoc = NULL;
	result = find_last_redir(node, &info, shell);
	if (result == 0)
		result = redir(&info, shell);
	if (result == -1)
		result = reset_fd(original_stdin, original_stdout, shell);
	close(original_stdin);
	close(original_stdout);
	return (result);
}
