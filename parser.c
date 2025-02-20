/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:36:58 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/18 18:36:58 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	node->redir_direction = -1;
	node->next = NULL;
	return (node);
}

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

static void redir_error(t_parser *parser, char *msg, t_node *redir_node)
{
	set_parser_error(parser, msg);
	free_node(redir_node);
}

void	parse_redirection(t_parser *parser, t_node **head)
{
	t_node			*redir_node;
	t_token_type	redir_type;

	redir_type = parser->current_token->type; // store the type of redirection
	redir_node = create_redir_node(redir_type);
	if (!redir_node)
	{
		set_parser_error(parser, "Failed to create redirection node");
		return ;
	}
	consume_token(parser); // move to the next token
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		if (redir_type == TOKEN_HEREDOC)
			// set_parser_error(parser, "Expected delimiter after heredoc redirection");
			redir_error(parser, "Expected delimiter after heredoc redirection", redir_node);
		else
			// set_parser_error(parser, "Expected filename after redirection");
			redir_error(parser, "Expected filename after redirection", redir_node);
		return ;
	}
	if (redir_type == TOKEN_HEREDOC)
		redir_node->heredoc_delim = ft_strdup(parser->current_token->value);
	else
		redir_node->redir_file = ft_strdup(parser->current_token->value);
	consume_token(parser);
	add_node_to_back(head, redir_node);
}

void parse_pipe(t_parser *parser, t_node **head)
{
	t_node *pipe_node;

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
		return ;
	}
	parse_simple_command(parser, head);
}

// this adds arguments to the command node as multiple strings
void	add_arg_to_command(char *arg, t_node *cmd_node)
{
	int		i;
	char	**new_args;

	if (!cmd_node->arg)
		return (0);
	i = 0;
	while (cmd_node->arg[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (cmd_node->arg[i])
	{
		new_args[i] = ft_strdup(cmd_node->arg[i]);
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd_node->arg);
	cmd_node->arg = new_args;
	return (1);
}

t_node	*create_command_node(char *cmd, t_node_type type)
{
	t_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->cmd = ft_strdup(cmd);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

void	handle_word_token(t_parser *parser, t_node **head, t_node **first_node)
{
	if (!*first_node)
	{
		*first_node = create_command_node(parser->current_token->value, NODE_COMMAND);
		add_node_to_back(head, *first_node);
	}
	else
		if (add_arg_to_command(parser->current_token->value, *first_node))
			set_parser_error(parser, "Failed to add argument to command");
	consume_token(parser);
}

void	parse_simple_command(t_parser *parser, t_node **head)
{
	t_node	*first_node;

	first_node = NULL;
	while (parser->current_token)
	{
		if (is_redir_token(parser->current_token->type))
			parse_redirection(parser, head);
		else if (parser->current_token->type == TOKEN_WORD)
			handle_word_token(parser, head, &first_node);
		else if (parser->current_token->type == TOKEN_PIPE)
		{
			parse_pipe(parser, head);
			break ;
		}
		else
			break ;
	}
}

t_node	*parse(t_parser *parser)
{
	t_node *head;

	head = NULL;
	if (!parser->token_list)
		return (NULL);
	while (parser->current_token && !parser->error)
	{
		parse_simple_command(parser, &head);
		if (parser->error)
			break ;
	}
	return (head);
}
