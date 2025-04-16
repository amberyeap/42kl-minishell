/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:03:51 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/16 11:41:07 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// if add new var with an empty value
void	add_var_empty(t_minishell *data, char *name, char *value)
{
	t_env	*new_var;

	new_var = create_new(name, value);
	if (!new_var)
		return ;
	add_env(data, new_var);
}

// check where it is a env
int	is_env(t_env *env, char *var_name)
{
	while (env)
	{
		if (strcmp(env->key, var_name) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

// Helper that can handle without equal sign
int	handle_other(char *arg, t_minishell *data)
{
	if (!valid_var_name(arg))
	{
		ft_fprintf(2, "Minishell: export: \'%s\': not a valid identifier\n",
			arg);
		return (1);
	}
	if (!is_env(data->env, arg))
		add_var_empty(data, arg, "");
	return (0);
}

// Helper that Handle argument with equal sign
int	handle_equal(char *equal, char *arg, t_minishell *data)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_substr(arg, 0, equal - arg);
	var_value = ft_strdup(equal + 1);
	if (!valid_var_name(var_name))
	{
		ft_fprintf(2, "Minishell: export: \'%s\': not a valid identifier\n",
			var_name);
		free(var_name);
		free(var_value);
		return (1);
	}
	else
		update_env_var(data, var_name, var_value);
	free(var_name);
	free(var_value);
	return (0);
}

// handle export with afew cases needs to listdown what it can handles later
//for the print export must got 2 condition for with = and w/o
int	export(char **args, t_minishell *data)
{
	int		i;
	char	*result;

	i = 1;
	if (!args[1])
	{
		sort_print_env(data);
		return (0);
	}
	while (args[++i])
	{
		result = ft_strchr(args[i], '=');
		if (result)
		{
			if (handle_equal(result, args[i], data) != 0)
				return (1);
		}
		else
		{
			if (handle_other(args[i], data) != 0)
				return (1);
		}
	}
	return (0);
}
