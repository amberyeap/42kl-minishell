/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:40:06 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 13:04:09 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_node(t_node *current_node, t_minishell *data)
{
	t_node *temp = data->parser_list;
	// 	while (temp)
	// 	{
    //     if (temp->cmd) // Check if cmd is not NULL
    //         printf("Executing command: %s\n", temp->cmd);
    //     else
    //         printf("Executing command: (null)\n");
    //     temp = temp->next;
	// 	}
		temp = data->parser_list;
		while (temp && temp->arg)
		{
        for (int i = 0; temp->arg[i]; i++)
            printf("Arg: %s\n", temp->arg[i]);
        temp = temp->next;
		}

	if (check_redir(current_node) == -1)
		return ;
	while (current_node)
	{
		if (current_node->type == NODE_PIPE)
			set_pipe(current_node, data);
		else
			execute_cmd(current_node, data);
		current_node = current_node->next;
	}
}

/*
	* The flow of this function is to create a child process to execute node
	# SIG_IGN is to ignore signal from the parentprocess
	# Then fork the process and restore the signal behaviour
	# run the execute node
	# Save the exit status, then restore the signal handler
*/
void	execute_fork(t_node *current_node, t_minishell *data)
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
		execute_node(current_node, data);
		exit(data->exit);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	signal(SIGINT, signal_handler);
}

/*
	* This process only for running the builtin
	* Since builtins run in parent process since it effect the shell state
*/
void	parent_execution(t_minishell *shell)
{
	t_node	*current_node;

	current_node = shell->parser_list;
	if (!current_node)
		return ;
	if (current_node->type == NODE_COMMAND)
	{
		if (check_builtin(current_node->cmd))
		{
			shell->exit = run_builtin(current_node->arg, shell);
			return ;
		}
	}
	execute_fork(current_node, shell);
}
