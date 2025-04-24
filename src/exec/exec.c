/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:40:06 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/25 00:29:59 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node *find_cmd_node(t_node *node)
{
	t_node *now = node;
	while (now)
	{
		if (now->type == NODE_COMMAND) // Or NODE_WORD if your structure uses that
			return (now);
		now = now->next;
	}
	return (NULL);
}

void	execute_node(t_node *node, t_minishell *data)
{
	t_node	*cmd_node;
	// print_parse_list(node);
	// check_redir(node, data);
	if (node->type == NODE_PIPE ||
		 (node->next && node->next->type == NODE_PIPE))
		set_pipe(node, data);
	else
	{
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
		if (check_redir_exists(data))
		{
			if (check_redir(node, data) == -1)
				// exit(1);
				data->exit = 1;
		}
		execute_node(node, data);
		exit(data->exit);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	signal(SIGINT, signal_handler);
}

void	parent_execution(t_minishell *shell)
{
	t_node	*node;
	int	stdin = -1;
	int	stdout = -1;
	t_node	*cmd_node;

	node = shell->parser_list;
	cmd_node = find_cmd_node(node);
	// if (node->type != NODE_COMMAND &&
	// 	!(node->next && node->next->type == NODE_PIPE) &&
	// 	check_redir_exists(shell))
	if (cmd_node == NULL && check_redir_exists(shell))
	{
		stdin = dup(STDIN_FILENO);
		stdout = dup(STDOUT_FILENO);

		if (check_redir(node, shell) == -1)
			shell->exit = 1;
		else
			shell->exit = 0;
		if (stdin != -1)
		{
			dup2(stdin, STDIN_FILENO);
			close(stdin);
		}
		if (stdout != -1)
		{
			dup2(stdout, STDERR_FILENO);
			close(stdout);
		}
		return ;
	}
	if (node->type == NODE_COMMAND &&
		check_builtin(node->cmd) &&
		!(node->next && node->next->type == NODE_PIPE))
		{
			shell->exit = run_builtin(node->arg, shell);
			return ;
		}
	execute_fork(node, shell);

}
