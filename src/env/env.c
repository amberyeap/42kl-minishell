/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:19:38 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:52:53 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	sort_print_env(t_minishell *data)
{
	char	**env_array;

	env_array = list_to_str(data->env);
	if (!env_array)
		return ;
	sorting_env(env_array);
	print_sort(env_array);
	free_array(env_array);
}

void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

// helper to create node for env list
t_env	*create_env(char *env_str)
{
	t_env	*node;
	char	*equal;

	if (!env_str)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal = ft_strchr(env_str, '=');
	if (!equal)
	{
		free(node);
		return (NULL);
	}
	node->key = ft_strndup(env_str, (equal - env_str));
	node->value = ft_strdup(equal + 1);
	node->next = NULL;
	return (node);
}

/*
	* This function is to initialize the environment variable
	* it goes check if the envp passed is not null(nothing)
	* it create a first node and check if the node is NULL
	* then it goes continue on creating new node for the env list
	* of during the process node creation fails it should free the node
	* then it should link the new node with previous, move env pointer

*/
int	init_env(t_minishell *data, char **envp)
{
	t_env	*env;
	t_env	*new;
	int		i;

	i = 0;
	if (!envp || !envp[0])
		return (1);
	env = create_env(envp[i]);
	if (!env)
		return (1);
	data->env = env;
	while (envp[++i])
	{
		new = create_env(envp[i]);
		if (!new)
		{
			free_env(data->env);
			return (1);
		}
		env->next = new;
		env = new;
	}
	return (0);
}
