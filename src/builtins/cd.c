/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:38:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/18 07:45:26 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	* so it look up the flag then it will check where 0(no args| ~) or 1(-)
	1) get_env_value check the the value of HOME if flag 0
	2) get_env_value check the value of OLDPWD if flag 1
	3) so if either is correct it should change directory depends on the flag
	4) then it update the oldpwd and pwd

*/
static int	go_to_path(int flag, t_minishell *data)
{
	char	*path;

	if (flag == 0)
	{
		path = get_env_value(data->env, "HOME");
		if (!path)
		{
			ft_fprintf(2, "Minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
	{
		path = get_env_value(data->env, "OLDPWD");
		if (!path)
		{
			ft_fprintf(2, "Minishel;: cd: OLDPWD not set\n");
			return (1);
		}
	}
	if (!change_directory(path))
		return (1);
	update_oldpwd(data);
	update_pwd(data);
	return (0);
}

/*
* @brief This is the main function for cd command
* @note There is several steps for handling cd:-
* 1) cd Main: calls go_to_path, then call chdir to change directory,
	updates OLDPWD and PWD after changing directory
* 2) go_to_path: handles special cases like no args, and -, update
	OLDPWD and PWD
* 3) update_oldpwd: updates OLPWD env with the current working directory
* 4) update_pwd: updates PWD env with the new working directory
*/
int	cd(char **args, t_minishell *data)
{
	int	ret_val;

	if (length(args) > 2)
	{
		ft_fprintf(2, "Minishell: cd: too many arguments\n");
		return (1);
	}
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (go_to_path(0, data));
	if (ft_strcmp(args[1], "-") == 0)
		return (go_to_path(1, data));
	update_oldpwd(data);
	ret_val = chdir(args[1]);
	if (ret_val == -1)
	{
		ft_fprintf(2, "Minishell: cd: %s: %s\n", args[1], strerror(errno));
		return (1);
	}
	update_pwd(data);
	return (0);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <errno.h>
// // #include "minishell.h"  // Include your minishell header file

// // Mock environment list for testing
// t_env *create_env_node(char *key, char *value)
// {
//     t_env *node = (t_env *)malloc(sizeof(t_env));
//     node->key = strdup(key);
//     node->value = strdup(value);
//     node->next = NULL;
//     return node;
// }

// t_env *create_test_env()
// {
//     t_env *env = create_env_node("HOME", "/home/user");
//     env->next = create_env_node("OLDPWD", "/tmp");
//     env->next->next = create_env_node("PWD", "/home/user");
//     return env;
// }

// void print_env(t_env *env)
// {
//     while (env)
//     {
//         printf("%s=%s\n", env->key, env->value);
//         env = env->next;
//     }
// }

// int main(void)
// {
//     t_minishell data;
//     char *args[3];

//     // Initialize the environment
//     data.env = create_test_env();

//     // Test 1: No argument (change to HOME)
//     printf("Test 1: cd (no argument)\n");
//     args[0] = "cd";
//     args[1] = NULL;
//     cd(args, &data);
//     print_env(data.env);
//     printf("\n");

//     // Test 2: Change to OLDPWD
//     printf("Test 2: cd -\n");
//     args[0] = "cd";
//     args[1] = "-";
//     args[2] = NULL;
//     cd(args, &data);
//     print_env(data.env);
//     printf("\n");

//     // Test 3: Change to a valid directory
//     printf("Test 3: cd /usr/bin\n");
//     args[0] = "cd";
//     args[1] = "/usr/bin";
//     args[2] = NULL;
//     cd(args, &data);
//     print_env(data.env);
//     printf("\n");

//     // Test 4: Change to an invalid directory
//     printf("Test 4: cd /invalid/path\n");
//     args[0] = "cd";
//     args[1] = "/invalid/path";
//     args[2] = NULL;
//     cd(args, &data);
//     print_env(data.env);
//     printf("\n");

//     // Test 5: Change to HOME using "~"
//     printf("Test 5: cd ~\n");
//     args[0] = "cd";
//     args[1] = "~";
//     args[2] = NULL;
//     cd(args, &data);
//     print_env(data.env);
//     printf("\n");

//     // Free the environment list
//     while (data.env)
//     {
//         t_env *temp = data.env;
//         data.env = data.env->next;
//         free(temp->key);
//         free(temp->value);
//         free(temp);
//     }

//     return 0;
// }
