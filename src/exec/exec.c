/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:31:09 by muhabin-          #+#    #+#             */
/*   Updated: 2025/03/23 02:31:12 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//check for pipe
int	is_pipe(t_node *node)
{
	return (node->type == NODE_PIPE);
}
//check for redirection should return if true
int	is_redirection(t_node *node)
{
	return (node->type == NODE_REDIR_IN ||
			node->type == NODE_REDIR_OUT ||
			node->type == NODE_APPEND ||
			node->type == NODE_HEREDOC);
}

//TODO: A function that check the cmd if it a redir, pipe, execve
//TODO: A function to check redirection and setup the redirection first
void	execute(t_minishell *data)
{
	if (is_redirection(data->parser_list))
		setup_redirection(data->parser_list);
	if (check_builtin(data->parser_list))
		run_builtin(data, data->parser_list);
	else if (is_pipe(data->parser_list))
		handle_pipe(data);
	else
		execute_cmd(data);
}
