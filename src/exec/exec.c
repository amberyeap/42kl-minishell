/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:40:06 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 02:26:22 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*find_cmd_node(t_node *node)
{
	t_node	*now;

	now = node;
	while (now)
	{
		if (now->type == NODE_COMMAND)
			return (now);
		now = now->next;
	}
	return (NULL);
}

void	execute_node(t_node *node, t_minishell *data)
{
	t_node	*cmd_node;

	if (node->type == NODE_PIPE
		|| (node->next && node->next->type == NODE_PIPE))
		set_pipe(node, data);
	else
	{
		if (check_redir(node, data) == -1)
			exit(1);
		cmd_node = find_cmd_node(node);
		execute_cmd(cmd_node, data);
	}
}

/*
	* The flow of this function is to create a child process to execute node
	# SIG_IGN is to ignore signal from the parentprocess
	# Then fork the process and restore the signal behaviour
	# run the execute node
	# Save the exit status, then restore the signal handler
*/
void	execute_fork(t_node *node, t_minishell *data)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_node(node, data);
		exit(data->exit);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	signal(SIGINT, signal_handler);
}

static void	redir_wo_cmd(t_node *node, t_minishell *shell)
{
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (check_redir(node, shell) == -1)
		shell->exit = 1;
	else
		shell->exit = 0;
	if (stdin_copy != -1)
	{
		dup2(stdin_copy, STDIN_FILENO);
		close(stdin_copy);
	}
	if (stdout_copy != -1)
	{
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
}

void	parent_execution(t_minishell *shell)
{
	t_node	*node;
	t_node	*cmd_node;

	node = shell->parser_list;
	cmd_node = find_cmd_node(node);
	if (cmd_node == NULL && check_redir_exists(shell))
	{
		redir_wo_cmd(node, shell);
		return ;
	}
	if (node->type == NODE_COMMAND
		&& check_builtin(node->cmd)
		&& !(node->next && node->next->type == NODE_PIPE))
	{
		shell->exit = run_builtin(node->arg, shell);
		return ;
	}
	execute_fork(node, shell);
}
