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

void	move_args(t_node *cmd_node, char **new_args, int *new_arg_expand)
{
	int	i;

	i = 0;
	while (cmd_node->arg[i])
	{
		new_args[i] = cmd_node->arg[i];
		new_arg_expand[i] = cmd_node->arg_expand[i];
		i++;
	}
	free(cmd_node->arg);
	free(cmd_node->arg_expand);
}

int	add_arg_to_command(char *word, int expand, t_node *cmd_node)
{
	int		i;
	char	**new_args;
	int		*new_arg_expand;

	i = 0;
	while (cmd_node->arg[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	new_arg_expand = malloc(sizeof(int) * (i + 2));
	if (!new_arg_expand)
		return (free(new_args), 0);
	move_args(cmd_node, new_args, new_arg_expand);
	new_args[i] = ft_strdup(word);
	new_args[i + 1] = NULL;
	new_arg_expand[i] = expand;
	cmd_node->arg = new_args;
	cmd_node->arg_expand = new_arg_expand;
	return (1);
}

t_node	*create_command_node(char *cmd, t_node_type type, int expand)
{
	t_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->arg = malloc(sizeof(char *) * 2);
	if (!node->arg)
		return (free(node), NULL);
	node->arg[0] = ft_strdup(cmd);
	if (!node->arg[0])
		return (free(node->arg), free(node), NULL);
	node->arg[1] = NULL;
	node->cmd = node->arg[0];
	node->expand = expand;
	node->arg_expand = malloc(sizeof(int) * 2);
	if (!node->arg_expand)
		return (free(node->arg[0]), free(node->arg), free(node), NULL);
	node->arg_expand[0] = expand;
	node->arg_expand[1] = 0;
	return (node);
}

void	handle_word_token(t_parser *parser, t_node **head, t_node **first_node)
{
	char	*word;
	int		expand;

	expand = parser->current_token->expand;
	word = ft_strdup(parser->current_token->value);
	if (!*first_node)
	{
		*first_node = create_command_node(word, NODE_COMMAND, expand);
		add_node_to_back(head, *first_node);
	}
	else
		if (!add_arg_to_command(word, expand, *first_node))
			set_parser_error(parser, "Failed to add argument to command");
	free(word);
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
