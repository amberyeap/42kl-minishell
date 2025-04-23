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

int	replace_args(t_node *cmd_node, char *arg, char ***new_args,
	char ***old_args, int *i)
{
	while (cmd_node->arg && cmd_node->arg[*i])
    {
        (*new_args)[*i] = ft_strdup(cmd_node->arg[*i]);
        if (!(*new_args)[*i])
        {
            while (--(*i) >= 0)
                free((*new_args)[*i]);
            free(*new_args);
            return (0);
        }
        (*i)++;
    }
    (*new_args)[*i] = ft_strdup(arg);
    (*new_args)[(*i) + 1] = NULL;
    *old_args = cmd_node->arg;
    *i = 0;
    while (*old_args && (*old_args)[*i])
        free((*old_args)[(*i)++]);
    free(*old_args);
	return (1);
}

// this adds arguments to the command node as multiple strings
int	add_arg_to_command(char *arg, t_node *cmd_node)
{
    int		i;
    char	**new_args;
    char	**old_args;

    i = 0;
    while (cmd_node->arg && cmd_node->arg[i])
        i++;
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return (0);
    i = 0;
    if (!replace_args(cmd_node, arg, &new_args, &old_args, &i))
		set_parser_error(cmd_node->shell->parser,
			"Error duplicating arguments");
    cmd_node->arg = new_args;
    return (1);
}

t_node	*create_command_node(char *cmd, t_node_type type)
{
	t_node	*node;
	char	**cmd_arg;

	cmd_arg = malloc(sizeof(char *) * 2);
	if (!cmd_arg)
		return (NULL);
	node = create_node(type);
	if (!node)
		return (free(cmd_arg), NULL);
	node->cmd = ft_strdup(cmd);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	cmd_arg[0] = ft_strdup(cmd);
	cmd_arg[1] = NULL;
	if (!cmd_arg[0])
		return (free(cmd_arg), free(node), NULL);
	node->arg = cmd_arg;
	return (node);
}

void	handle_word_token(t_parser *parser, t_node **head, t_node **first_node)
{
	char	*word;

	if (parser->current_token->expand)
		word = expand_var(parser->current_token->value, parser->shell->exit);
	else
		word = ft_strdup(parser->current_token->value);
	if (!*first_node)
	{
		*first_node = create_command_node(word, NODE_COMMAND);
		add_node_to_back(head, *first_node);
	}
	else
		if (!add_arg_to_command(word, *first_node))
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
		if (parser->error)
		{
			if (first_node)
				free_node(first_node);
			return;
		}
	}
}
