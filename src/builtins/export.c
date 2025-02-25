/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:03:51 by muhabin-          #+#    #+#             */
/*   Updated: 2025/02/25 15:20:24 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// handle export with afew cases needs to listdown what it can handles later
int	export(char **args, t_env *env, t_env *backup_env)
{
	int	new_env;
	int	valid_res;

	new_env = 0;
	if (!args[1])// if no argument, print sorted temp_env and return
	{
		//print_sorted_env(backup_env);
		return (0);
	}
	valid_res = valid_env(args[1]);// validate the environment variable argument
	if (args[1][0] == '=')
		valid_res = -3;// handle invalid case where = is the start or args
	if (valid_res <= 0)// if argument is invalid, print error and return
		return (print_error(valid_res, args[1]));
	new_env = exist_env(env, args[1]);//returns 1 if updated, return 0 if not found
	if (!new_env)// if dont exist
	{
		if (valid_res == 1)
			add_env(args[1],env);
		add_env(args[1], backup_env);
	}
	return (0);
}
