/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:03:09 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/20 16:50:36 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this adds arguments to the command node as multiple strings
int	add_arg_to_command(char *arg, t_node *cmd_node)
{
	int		i;
	char	**new_args;

	// if (!cmd_node->arg)
	// 	return (0);
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

char *get_env_var(char **value, char *var_start, char *result, int *i)
{
	char	*expanded_var;
	char	*var_name;

	(*value)++;
	while (ft_isalnum(**value))
		(*value)++;
	var_name = ft_substr(*value, 0, *value - var_start);
	expanded_var = getenv(var_name);
	if (expanded_var)
		*i += ft_strlcpy(result + (*i), expanded_var, ft_strlen(expanded_var) + 1);
	free(var_name);
	return (result);
}

char	*expand_var(char *value)
{
	char	*var_start;
	char	*result;
	int		i;

	i = 0;
	result = malloc(1024);
	if (!result)
		return (NULL);
	var_start = ft_strchr(value, '$');
	if (!var_start)
		return (free(result), ft_strdup(value));
	while (*value)
	{
		if (*value == '$' && ft_isalnum(*(value + 1)) && *(value + 1))
		{
			if (!get_env_var(&value, var_start, result, &i))
				return (free(result), NULL);
		}
		else
			result[i++] = *value++;
	}
	result[i] = '\0';
	return (result);
}

void	handle_word_token(t_parser *parser, t_node **head, t_node **first_node)
{
	char	*expanded_arg;

	expanded_arg = expand_var(parser->current_token->value);
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
