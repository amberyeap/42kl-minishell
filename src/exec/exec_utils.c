/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:31:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/17 21:38:57 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_pipe(t_node *node, t_minishell *data)
{
	int		pipe_fd[2];
	int		prev_pipe_in = -1;
	pid_t	pid;
	t_node	*current = node;

	while (current != NULL)
	{
		if (current->next != NULL && current->next->type == NODE_PIPE)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell: pipe");
				data->exit = EXIT_FAILURE;
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			data->exit = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_pipe_in != -1)
			{
				if (dup2(prev_pipe_in, STDIN_FILENO) == -1)
				{
					perror("minishell: dup2");
					data->exit = EXIT_FAILURE;
					exit(EXIT_FAILURE);
				}
				close(prev_pipe_in);
			}
			if (current->next != NULL && current->next->type == NODE_PIPE)
			{
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				{
					perror("minishell: dup2");
					exit(EXIT_FAILURE);
				}
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			execute_cmd(current, data);
			exit(data->exit);
		}
		else
		{
			if (prev_pipe_in != -1)
				close(prev_pipe_in);
			if (current->next != NULL && current->next->type == NODE_PIPE)
			{
				prev_pipe_in = pipe_fd[0];
				close(pipe_fd[1]);
			}
			else
				prev_pipe_in = -1;
			current = current->next;
			if (current != NULL && current->type == NODE_PIPE)
				current = current->next;
		}
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}
