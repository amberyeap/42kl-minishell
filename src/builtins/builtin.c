/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:55:25 by muhabin-          #+#    #+#             */
/*   Updated: 2025/02/18 18:09:53 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"


//check the input whether it is a commmand or not
int	check_builtin(char *command)
{
	if (ft_strncmp(command, "echo", 4) == 0)
		return (1);
	if (ft_strncmp(command, "cd ", 2) == 0)
		return (1);
	if (ft_strncmp(command, "pwd", 3) == 0)
		return (1);
	if (ft_strncmp(command, "env", 3) == 0)
		return (1);
	if (ft_strncmp(command, "export", 6) == 0)
		return (1);
	if (ft_strncmp(command, "unset", 5) == 0)
		return (1);
	return (0);
}
//run the builtinafter being check
int	run_builtin(char **args, t)
