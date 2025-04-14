/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:55:25 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/14 13:26:30 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// ECHO BUGS
// [FIXED] BUG: only prints a newline when there is only one argument
// [FIXED] BUG: segfaults when there is no argument input (eg: echo ), works for echo ""
// [FIXED BUT FUNCTION TOO LONG] BUG: doesn't print "hello"world in one line, but as two separate strings
// TO TEST: the skip_flags()

// CD BUGS
// - 'cd ..' goes back to the home/USER path
// - 'cd <path> doesn't work

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
// int	run_builtin(char **args, t_minishell *data)
// {
// 	int	result;

// 	printf("entered run_builtin\n");
// 	result = 0;
// 	if (ft_strcmp(args[0], "echo") == 0)
// 		result = ft_echo(args);
// 	if (ft_strcmp(args[0], "cd") == 0)
// 		result = cd(args, data);
// 	if (ft_strcmp(args[0], "pwd") == 0)
// 		result = pwd(args);
// 	if (ft_strcmp(args[0], "env") == 0)
// 		result = ft_env(args, data->env);
// 	if (ft_strcmp(args[0], "export") == 0)
// 		result = export(args, data);
// 	if (ft_strcmp(args[0], "unset") == 0)
// 		result = unset(args, data);
// 	if (ft_strcmp (args[0], "exit") == 0)
// 		ft_exit(data, args);
// 	return (result);
// }

int	run_builtin(char **args, t_minishell *data)
{
	int	result;

	result = 0;
	if (ft_strcmp(data->parser_list->cmd, "echo") == 0)
		result = ft_echo(args);
	else if (ft_strcmp(data->parser_list->cmd, "cd") == 0)
		result = cd(args, data);
	else if (ft_strcmp(data->parser_list->cmd, "pwd") == 0)
		result = pwd(args);
	else if (ft_strcmp(data->parser_list->cmd, "env") == 0)
		result = ft_env(args, data->env);
	else if (ft_strcmp(data->parser_list->cmd, "export") == 0)
		result = export(args, data);
	else if (ft_strcmp(data->parser_list->cmd, "unset") == 0)
		result = unset(args, data);
	else if (ft_strcmp(data->parser_list->cmd, "exit") == 0)
		result = ft_exit(data, args);
	return (result);
}

// Helper function to trim whitespace from both ends of a string
// char *trim_whitespace(char *str)
// {
//     if (!str) return NULL;

//     // Trim leading space
//     while (isspace((unsigned char)*str)) str++;

//     if (*str == '\0') return str; // All spaces?

//     // Trim trailing space
//     char *end = str + strlen(str) - 1;
//     while (end > str && isspace((unsigned char)*end)) end--;

//     // Write new null terminator
//     *(end + 1) = '\0';

//     return str;
// }

// int run_builtin(char **args, t_minishell *data)
// {
//     if (!data || !data->parser_list || !data->parser_list->cmd)
//         return (1); // Error: Invalid data

//     // Debug prints (keep these temporarily)
//     printf("Raw command: '%s'\n", data->parser_list->cmd);
//     printf("strcmp('env', cmd) = %d\n", strcmp(data->parser_list->cmd, "env"));

//     // Trim whitespace from command
//     char *trimmed_cmd = trim_whitespace(data->parser_list->cmd);
//     printf("Trimmed command: '%s'\n", trimmed_cmd);

// 	printf("Command bytes: ");
// 	for (int i = 0; data->parser_list->cmd[i]; i++)
// 		printf("%d ", data->parser_list->cmd[i]);

//     int result = -1; // Default: not a built-in command

//     if (ft_strncmp(trimmed_cmd, "echo", 4) == 0)
//         result = ft_echo(args);
//     else if (ft_strncmp(trimmed_cmd, "cd", 2) == 0)
//         result = cd(args, data);
//     else if (ft_strncmp(trimmed_cmd, "pwd", 3) == 0)
//         result = pwd(args);
//     else if (ft_strncmp(trimmed_cmd, "env", 3) == 0) {
//         printf("Command matched: env\n");
//         result = ft_env(args, data->env);
//     }
//     else if (ft_strncmp(trimmed_cmd, "export", 6) == 0)
//         result = export(args, data);
//     else if (ft_strncmp(trimmed_cmd, "unset", 5) == 0)
//         result = unset(args, data);
//     else if (ft_strncmp(trimmed_cmd, "exit", 4) == 0)
//         ft_exit(data, args); // ft_exit should return int

//     // Debug prints for args (keep temporarily)
//     if (args) {
//         printf("Command arguments:\n");
//         for (int i = 0; args[i]; i++)
//             printf("  arg[%d] = '%s'\n", i, args[i]);
//     }

//     return result;
// }
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
