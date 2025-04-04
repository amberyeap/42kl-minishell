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

char	*get_env_var(char **value, char *var_start, char *result, int *i)
{
	char	*expanded_var;
	char	*var_name;

	(*value)++;
	while (ft_isalnum(**value))
		(*value)++;
	var_name = ft_substr(*value, 0, *value - var_start);
	expanded_var = getenv(var_name);
	if (expanded_var)
		*i += ft_strlcpy(result + (*i), expanded_var,
				ft_strlen(expanded_var) + 1);
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
