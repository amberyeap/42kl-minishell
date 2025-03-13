/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:34:16 by muhabin-          #+#    #+#             */
/*   Updated: 2025/03/13 11:19:00 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_memdel(void *ptr)
{
	free(*(void **)ptr);
	*(void **)ptr = NULL;
}

int	valid_var_name(char *var_name)
{
	int	i;

	if (!var_name || (!ft_isalpha(var_name[0]) && var_name[0] != '_'))
		return (0);
	i = 0;
	while (var_name[i] && var_name[i] != '=')
	{
		if (!isalnum(var_name[i]) && var_name[i] != '_')
			return (0);
		i++;
	}
	return (1);// Success
}
// Adds the new environment variable into the list
void	add_env(t_minishell *data, t_env *new_var)
{
	t_env	*current;

	if (!data->env)//if the list in empty set new var as the head
	{
		data->env = new_var;
		return;
	}
	// traverse to the end of the list and then add
	current = data->env;
	while (current->next)
		current = current->next;
	current->next = new_var;
}
// This helper create a new environment variable node
t_env	*create_new(char *var_name, char *var_value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));// malloc for the new
	if (!new_env)
		return (NULL);
	new_env->key = ft_strdup(var_name);
	new_env->value = ft_strdup(var_value);
	if (!new_env->key || !new_env->value)
	{
		free(new_env->key);
		free(new_env->value);
		free(new_env);
		return (NULL);
	}
	new_env ->next = NULL;//initialize the next pointer
	return (new_env);
}

/*
*	@brief This function update the env but at the same time it check:
	@note if the variable exist or have create new var or update

	:-
	1) It iterates the environment list and check if already exist
	2) If the variable exist, update its value(after =)
	3) If the variable don't exist, add to env list
	4) Should hanlde memory, return
*/
void	update_env_var(t_minishell *data, char *var_name, char *var_value)
{
	t_env	*current;
	t_env	*new_var;

	current = data->env;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)// strcmp the key(ori) with var_name(input)
		{
			free(current->value);// update the old value with the new value, is the same also
			current->value = ft_strdup(var_value);// Assign new value
			if(!current->value)
				return;
			return;
		}
		current= current->next;
	}
	new_var = create_new(var_name,var_value);// if the var don't exist, add into env list
	if(!new_var)
		return;
	add_env(data,new_var);
}
