/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:45:45 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 11:09:38 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_env(char **args, t_env *env)
{
	t_env	*current;

	current = env;
	if (length(args) > 1)
	{
		ft_putstr_fd("env: too many arguments", 2);
		return (1);
	}
	if (!current)
	{
		ft_putstr_fd("env: environment not initialized\n", 2);
		return (1);
	}
	while (current)
	{
		if (current->key && current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
