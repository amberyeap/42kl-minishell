/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:24:39 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 21:24:49 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_var(char **value, char *result, int *i, t_minishell *shell)
{
	char	*expanded_var;
	char	*var_name;
	char	*start;

	start = *value;
	(*value)++;
	while (ft_isalnum(**value))
		(*value)++;
	var_name = ft_substr(start + 1, 0, *value - (start + 1));
	if (!var_name)
		return (NULL);
	expanded_var = get_env_value(shell->env, var_name);
	if (expanded_var)
		*i += ft_strlcpy(result + (*i), expanded_var,
				ft_strlen(expanded_var) + 1);
	free(var_name);
	return (result);
}

void	expand_exit_status(int exit_status, char **value, char *result, int *i)
{
	char	*exit_str;

	exit_str = ft_itoa(exit_status);
	*i += ft_strlcpy(result + (*i), exit_str, ft_strlen(exit_str) + 1);
	free(exit_str);
	*value += 2;
}

char	*expand_var(char *value, int exit_status, t_minishell *shell)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(1024);
	if (!result)
		return (NULL);
	while (*value)
	{
		if (*value == '$' && *(value + 1) == '?')
			expand_exit_status(exit_status, &value, result, &i);
		else if (*value == '$' && *(value + 1) == '!')
			value += 2;
		else if (*value == '$' && ft_isalnum(*(value + 1)))
		{
			if (!get_env_var(&value, result, &i, shell))
				return (free(result), NULL);
		}
		else
			result[i++] = *value++;
	}
	result[i] = '\0';
	return (result);
}

void	expand_str(char **str, int expand, t_minishell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_var(*str, shell->exit, shell);
		if (!expanded)
		{
			free(*str);
			*str = NULL;
			return ;
		}
		free(*str);
		*str = expanded;
	}
}

void	expand_list(t_minishell *shell, t_node *parser_list)
{
	int		i;
	t_node	*current;

	current = parser_list;
	while (current)
	{
		if (current->arg)
		{
			i = 0;
			while (current->arg[i])
			{
				expand_str(&current->arg[i], current->arg_expand[i], shell);
				i++;
			}
		}
		if (current->cmd)
			current->cmd = current->arg[0];
		current = current->next;
	}
}
