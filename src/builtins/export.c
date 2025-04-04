/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:03:51 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/04 14:40:01 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// if add new var with an empty value
void add_var_empty(t_minishell *data, char *name, char *value)
{
	t_env	*new_var;

	new_var = create_new(name, value);
	if (!new_var)
		return;
	add_env(data, new_var);
}
// check where it is a env
int	is_env(t_env *env, char *var_name)
{
	while(env)
	{
		if (strcmp(env->key, var_name) == 0)
			return (1);//succes exist
		env = env->next;
	}
	return (0);// not exist
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
	if (!is_env(data->env,arg))
		add_var_empty(data, arg, "");
	return (0);
}
// Helper that Handle argument with equal sign
int	handle_equal(char *equal, char *arg, t_minishell *data)
{
	char *var_name;//before = (key)
	char *var_value;// after =

	//split the argument into variable name and value
	var_name = ft_substr(arg, 0, equal - arg);// extract before =
	var_value = ft_strdup(equal + 1);// extract value after =
	if (!valid_var_name(var_name))// check if the variable name is valid
	{
		ft_fprintf(2, "Minishell: export: \'%s\': not a valid identifier\n",
			var_name);
		free(var_name);
		free(var_value);
		return (1);
	}
	else// if valid
		update_env_var(data, var_name,var_value);// goes here to check if new or update
	free(var_name);
	free(var_value);
	return (0);
}

// handle export with afew cases needs to listdown what it can handles later
//for the print export must got 2 condition for with = and w/o
int	export(char **args, t_minishell *data)
{
	int		i;// iterates the argument
	char	*result; // stores if their is =

	i = 0;
	if (!args[1])// if no argument, print export variable
	{
		sort_print_env(data);
		return (0);// if the print is a success should return 0
	}
	while (args[++i])// iterates through each argument skips 0
	{
		result = ft_strchr(args[i], '=');//check if argument contains an =
		if (result)
		{
			if (handle_equal(result, args[i], data) != 0) // handle variable assignment
				return (1);// return error
		}
		else
		{
			if (handle_other(args[i], data) != 0)// Handle variable export without assignment
				return (1);
		}
	}
	return (0);
}

// #include <stdio.h>

// void free_env(t_env *env)
// {
//     t_env *tmp;

//     while (env)
//     {
//         tmp = env;
//         env = env->next;
//         free(tmp->key);
//         free(tmp->value);
//         free(tmp);
//     }
// }
// void free_split(char **arr)
// {
//     int i;

//     i = 0;
//     while (arr[i])
//     {
//         free(arr[i]);
//         i++;
//     }
//     free(arr);
// }
// void print_env(t_env *env)
// {
//     while (env)
//     {
//         printf("%s=%s\n", env->key, env->value);
//         env = env->next;
//     }
// }
// void test_export(t_minishell *data, char *input)
// {
//     char **args;
//     int result;

//     // Simulate command-line arguments
//     args = ft_split(input, ' ');
//     if (!args)
//     {
//         printf("Error: Failed to split input\n");
//         return;
//     }

//     // Call the export function
//     result = export(args, data);

//     // Print the result
//     printf("Input: %s\n", input);
//     printf("Result: %d\n", result);
//     printf("Environment:\n");
//     print_env(data->env); // Assuming you have a function to print the environment
//     printf("\n");

//     // Free allocated memory
//     free_split(args);
// }

// int main(void)
// {
//     t_minishell data;

//     // Initialize the environment (you can add some default variables here)
//     data.env = NULL;

//     // Test cases
//     test_export(&data, "export");
//     test_export(&data, "export MY_VAR=123");
//     test_export(&data, "export MY_VAR");
//     test_export(&data, "export 1VAR=123");
//     test_export(&data, "export MY_VAR=456");
//     test_export(&data, "export MY_VAR=\"hello world\"");
//     test_export(&data, "export MY_VAR=123 ANOTHER_VAR=456");
//     test_export(&data, "export MY_VAR=123 1VAR=456");
//     test_export(&data, "export MY_VAR=");
//     test_export(&data, "export _MY_VAR=123");

//     // Free the environment
//     free_env(data.env);

//     return 0;
// }
