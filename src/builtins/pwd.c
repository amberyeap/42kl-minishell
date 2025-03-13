/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:05:07 by muhabin-          #+#    #+#             */
/*   Updated: 2025/02/19 12:58:31 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int	length(char **arg)
// {
// 	int	i;

// 	i = 0;
// 	if (arg == NULL)
// 		return (0);
// 	while (arg[i])
// 		i++;
// 	return (i);
// }
/*
 * @brief this function uses getcwd then print the current working directory
*/
int	pwd(char **args)
{
	char	cwd[PATH_MAX];

	if (length(args) > 1)
	{
		ft_putstr_fd("pwd: too many arguments",STDERR_FILENO);
	}
	if (getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
		perror("pwd");
		return (1);
}

// int main(void)
// {
// 	if(pwd(NULL) != 0)
// 	{
// 		printf("error\n");
// 		return 1;
// 	}
// 	return 0;
// }
