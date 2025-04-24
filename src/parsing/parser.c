/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:36:58 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/20 14:03:47 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*create_node(t_node_type type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = NULL;
	node->arg = NULL;
	node->heredoc_delim = NULL;
	node->redir_file = NULL;
	node->expand = 0;
	node->arg_expand = NULL;
	node->quoted = false;
	node->next = NULL;
	return (node);
}

void	parse_pipe(t_parser *parser, t_node **head)
{
	t_node	*pipe_node;

	if (!*head)
	{
		set_parser_error(parser, "Expected command before pipe");
		return ;
	}
	pipe_node = create_node(NODE_PIPE);
	if (!pipe_node)
	{
		set_parser_error(parser, "Failed to create pipe node");
		return ;
	}
	add_node_to_back(head, pipe_node);
	consume_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		set_parser_error(parser, "Expected command after pipe");
		free(pipe_node);
		return ;
	}
	parse_simple_command(parser, head);
}

t_node	*parse(t_parser *parser)
{
	t_node	*head;

	head = NULL;
	if (!parser->token_list)
		return (NULL);
	while (parser->current_token && !parser->error)
	{
		parse_simple_command(parser, &head);
		if (parser->error)
			break ;
	}
	if (parser->error)
	{
		free_nodes_list(head);
		return (NULL);
	}
	return (head);
}
