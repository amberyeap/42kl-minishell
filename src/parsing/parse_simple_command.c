/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:03:09 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 21:24:38 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// this adds arguments to the command node as multiple strings
int	add_arg_to_command(char *arg, t_node *cmd_node)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd_node->arg && cmd_node->arg[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (cmd_node->arg && cmd_node->arg[i])
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
	char	*expanded_arg;

	expanded_arg = expand_var(parser->current_token->value, parser->shell->exit);
	if (!*first_node)
	{
		*first_node = create_command_node(expanded_arg, NODE_COMMAND);
		add_node_to_back(head, *first_node);
	}
	else
		if (!add_arg_to_command(expanded_arg, *first_node))
			set_parser_error(parser, "Failed to add argument to command");
	free(expanded_arg);
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
