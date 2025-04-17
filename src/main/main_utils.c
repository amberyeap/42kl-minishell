/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:16:47 by ayeap             #+#    #+#             */
/*   Updated: 2025/04/17 15:52:38 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_shell(t_minishell *shell, char **env)
{
	shell->user_input = NULL;
	(void)env;
	shell->exit = 0;
	shell->token_list = NULL;
	shell->parser_list = NULL;
	shell->parser = NULL;
}

void	reset_main(t_minishell *shell)
{
	free_token_list(shell->token_list);
	free(shell->user_input);
	if (shell->parser_list)
		free_nodes_list(shell->parser_list);
	shell->parser = NULL;
	shell->parser_list = NULL;
	shell->token_list = NULL;
	shell->user_input = NULL;
}

int	check_token_error(t_minishell *shell)
{
	t_token	*current;

	current = shell->token_list;
	while (current)
	{
		if (current->type == TOKEN_ERROR)
		{
			ft_fprintf(STDERR_FILENO, "minishell: syntax error: %s\n", current->value);
			free_token_list(shell->token_list);
			shell->token_list = NULL;
			shell->exit = 2;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	cleanup_parser(t_minishell *shell)
{
	free_token_list(shell->token_list);
	free(shell->user_input);
	shell->token_list = NULL;
	if (shell->parser && shell->parser->error_msg)
	{
		free(shell->parser->error_msg);
		shell->parser->error_msg = NULL;
	}
	shell->user_input = NULL;
}
