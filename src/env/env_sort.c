/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:31:10 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:50:43 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_sort(char **sorted_env)
{
	int		i;
	char	*equal;

	i = 0;
	while (sorted_env[i])
	{
		equal = ft_strchr(sorted_env[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", sorted_env[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
}

void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

static int	count_node(t_env *env)
{
	int	i;

	i = 0;
	while (env != NULL)
	{
		env = env->next;
		i++;
	}
	return (i);
}

/*
	* How it works converting list node to array?
		#need to count the list node first
		#malloc array then + 1 for NULL
		#join the key with "=" in the array
			array[0] = PATH=
		#join the value later
			array[0] = PATH=/usr/bin
*/
char	**list_to_str(t_env *env)
{
	int		count;
	char	**array;
	int		i;
	char	*temp;

	count = count_node(env);
	array = malloc(sizeof(char *) * (count + 1));
	if (!env || !array)
		return (NULL);
	i = 0;
	while (env)
	{
		temp = ft_strjoin(env->key, "=");
		array[i] = ft_strjoin(temp, env->value);
		free(temp);
		if (!array[i])
		{
			free_array(array);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

void	sorting_env(char **array_env)
{
	int		i;
	int		j;
	char	*temp;

	if (!array_env || !array_env[0])
		return ;
	i = 0;
	while (array_env[i])
	{
		temp = array_env[i];
		j = i - 1;
		while (j >= 0 && ft_strcmp(array_env[j], temp) > 0)
		{
			array_env[j + 1] = array_env[j];
			j--;
		}
		array_env[j + 1] = temp;
		i++;
	}
}
