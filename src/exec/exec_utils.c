/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:31:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:31:47 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	wait_for_child(pid_t last_pid, t_minishell *data)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (WIFEXITED(status) && pid == last_pid)
			data->exit = WEXITSTATUS(status);
		pid = waitpid(-1, &status, 0);
	}
}

static void	parent_fds(int *prev_pipe_in, int *pipe_fd, int has_pipe)
{
	if (*prev_pipe_in != -1)
		close(*prev_pipe_in);
	if (has_pipe)
		*prev_pipe_in = pipe_fd[0];
	else
		*prev_pipe_in = -1;
	if (has_pipe)
		close(pipe_fd[1]);
}

static int	birthing_pipe(int *pipe_fd, int has_pipe)
{
	if (has_pipe)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("minishell: pipe");
			return (-1);
		}
	}
	return (0);
}

static void	fork_n_exec(t_node *node, t_exec *info, t_minishell *data)
{
	if (info->prev_pipe_in != -1)
	{
		dup2(info->prev_pipe_in, STDIN_FILENO);
		close(info->prev_pipe_in);
	}
	if (info->has_pipe)
	{
		dup2(info->pipe_fd[1], STDOUT_FILENO);
		close(info->pipe_fd[0]);
		close(info->pipe_fd[1]);
	}
	if (check_redir(node, data) == -1)
		exit(1);
	execute_cmd(node, data);
	exit(data->exit);
}

void	set_pipe(t_node *node, t_minishell *data)
{
	t_node	*current;
	t_exec	info;
	pid_t	last_pid;
	pid_t	pid;

	current = node;
	info.prev_pipe_in = -1;
	last_pid = -1;
	while (current)
	{
		info.has_pipe = 0;
		if (current->next && current->next->type == NODE_PIPE)
			info.has_pipe = 1;
		if (birthing_pipe(info.pipe_fd, info.has_pipe) == -1)
			return ;
		pid = fork();
		if (pid == 0)
			fork_n_exec(current, &info, data);
		last_pid = pid;
		parent_fds(&info.prev_pipe_in, info.pipe_fd, info.has_pipe);
		current = movin_node(current, info.has_pipe);
	}
	wait_for_child(last_pid, data);
}
