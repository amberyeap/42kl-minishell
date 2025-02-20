/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:03:48 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/20 14:03:59 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_redir_node(t_token_type type)
{
	t_node	*redir_node;

	redir_node = NULL;
	if (type == TOKEN_APPEND)
	{
		redir_node = create_node(NODE_APPEND);
		redir_node->redir_direction = OUTPUT;
	}
	else if (type == TOKEN_REDIR_IN)
	{
		redir_node = create_node(NODE_REDIR_IN);
		redir_node->redir_direction = INPUT;
	}
	else if (type == TOKEN_REDIR_OUT)
	{
		redir_node = create_node(NODE_REDIR_OUT);
		redir_node->redir_direction = OUTPUT;
	}
	else if (type == TOKEN_HEREDOC)
	{
		redir_node = create_node(NODE_HEREDOC);
		redir_node->redir_direction = INPUT;
	}
	return (redir_node);
}

static void	redir_error(t_parser *parser, char *msg, t_node *redir_node)
{
	set_parser_error(parser, msg);
	free_node(redir_node);
}

void	parse_redirection(t_parser *parser, t_node **head)
{
	t_node			*redir;
	t_token_type	redir_type;

	redir_type = parser->current_token->type;
	redir = create_redir_node(redir_type);
	if (!redir)
	{
		set_parser_error(parser, "Failed to create redirection node");
		return ;
	}
	consume_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		if (redir_type == TOKEN_HEREDOC)
			redir_error(parser, "Expected delimiter after '<<'", redir);
		else
			redir_error(parser, "Expected filename after redirection", redir);
		return ;
	}
	if (redir_type == TOKEN_HEREDOC)
		redir->heredoc_delim = ft_strdup(parser->current_token->value);
	else
		redir->redir_file = ft_strdup(parser->current_token->value);
	consume_token(parser);
	add_node_to_back(head, redir);
}
