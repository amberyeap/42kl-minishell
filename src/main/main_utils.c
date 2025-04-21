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

void    reset_main(t_minishell *shell)
{
	if (shell->user_input)
	{
		free(shell->user_input);
		shell->user_input = NULL;
	}
	if (shell->token_list)
		free_token_list(shell->token_list);
	if (shell->parser_list)
		free_nodes_list(shell->parser_list);
	cleanup_parser(shell);
	shell->token_list = NULL;
	shell->parser_list = NULL;
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

void    cleanup_parser(t_minishell *shell)
{
	if (shell->parser)
	{
		if (shell->parser->error_msg)
			free(shell->parser->error_msg);
		free(shell->parser);
		shell->parser = NULL;
	}
}
