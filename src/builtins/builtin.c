/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:55:25 by muhabin-          #+#    #+#             */
/*   Updated: 2025/03/25 00:19:43 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	if (ft_strncmp(command, "exit", 4) == 0)
		return (1);
	if (ft_strncmp(command, "export", 6) == 0)
		return (1);
	if (ft_strncmp(command, "unset", 5) == 0)
		return (1);
	return (0);
}

//run the builtin after being check
int	run_builtin(char **args, t_minishell *data)
{
	int	result;

	result = 0;
	if (ft_strcmp(args[0], "echo") == 0)
		result = ft_echo(args);
	if (ft_strcmp(args[0], "cd") == 0)
		result = cd(args, data->env);
	if (ft_strcmp(args[0], "pwd") == 0)
		result = pwd(args);
	if (ft_strcmp(args[0], "env") == 0)
		result = env(args,data->env);
	if (ft_strcmp(args[0], "export") == 0)
		result = export(args, data->env);
	if (ft_strcmp(args[0], "unset") == 0)
		result = unset(args, data);
	if (ft_strcmp (args[0], "exit") == 0)
		result = ft_exit(args, data);
	return (result);
}
// int ft_strncmp(const char *s1, const char *s2, size_t n)
// {
//     while (n && *s1 && (*s1 == *s2))
//     {
//         s1++;
//         s2++;
//         n--;
//     }
//     if (n == 0)
//         return 0;
//     return (*(unsigned char *)s1 - *(unsigned char *)s2);
// }

// int main()
// {
// 	if (check_builtin("psd") == 1)
// 		printf("correct");
// 	else
// 		printf("not correct");
// }
