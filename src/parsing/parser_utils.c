/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:36:44 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/20 14:05:52 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_redir_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (1);
	return (0);
}

void	consume_token(t_parser *parser)
{
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
}

void	set_parser_error(t_parser *parser, char *msg)
{
	parser->error = 1;
	parser->error_msg = ft_strdup(msg);
}

void	add_node_to_back(t_node **head, t_node *new_node)
{
	t_node	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

// initializes the parser
t_parser	*init_parser(t_token *token_list)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->token_list = token_list;
	parser->current_token = token_list;
	parser->error = 0;
	parser->error_msg = NULL;
	return (parser);
}
