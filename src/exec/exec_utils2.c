/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/24 15:01:31 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}
char	*find_path(char *cmd, t_env *env)
{
	char	*path;
	char	*full_path;
	char	**paths;
	int		i;
	char	*temp;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free_paths(paths);
	return (NULL);
}

// void	execute_cmd(t_node *node, t_minishell *data)
// {
// 	char	*cmd;
// 	char	**env;
// 	int		result;

// 	if (check_builtin(node->cmd))
// 	{
// 		handle_builtin(node->arg, data, &result);
// 		exit(result);
// 	}
// 	if (!node->arg || !node->arg[0])
// 	{
// 		ft_fprintf(STDERR_FILENO, "minishell: empty command\n");
// 		data->exit = 127;
// 		return ;
// 	}
// 	cmd = find_path(node->cmd, data->env);
// 	if (cmd == NULL)
// 	{
// 		ft_fprintf(STDERR_FILENO, "%s: command not found\n", node->cmd);
// 		data->exit = 127;
// 		exit(127);
// 	}
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, F_OK) != 0) //Files not exit
// 		{
// 			fprintf(stderr, "%s: command not found\n", cmd);
// 			data->exit = 127;
// 			exit(127);
// 		}
// 		if (access(cmd, X_OK) != 0) // not executable
// 		{
// 			fprintf(stderr, "%s: Permission denied\n", cmd);
// 			data->exit = 126;
// 			exit(126);
// 		}
// 		if (is_directory(cmd))
// 		{
// 			ft_fprintf(2, "%s: Is a directory\n", cmd);
// 			data->exit = 126;
// 			exit(126);
// 		}
// 	}
// 	else
// 	{
// 		perror("Command not found");
// 		data->exit = 126;
// 		return ;
// 		cmd = find_path(cmd, data->env);
// 		if (cmd == NULL)
// 		{
// 			ft_fprintf(2, "%s: command not found\n", node->cmd);
// 			data->exit = 127;
// 			exit(127);
// 		}
// 	}
// 	env = list_to_str(data->env);
// 	if (execve(cmd, node->arg, env) == -1)
// 	{
// 		fprintf(stderr, "%s: command not found\n", node->cmd);
// 		data->exit = 127;
// 		exit(127);
// 	}
// }

// void	execute_cmd(t_node *node, t_minishell *data)
// {
// 	char	*cmd;
// 	char	**env;
// 	int		result;

// 	if (check_builtin(node->cmd))
// 	{
// 		handle_builtin(node->arg, data, &result);
// 		exit(result);
// 	}
// 	if (!node->arg || !node->arg[0])
// 	{
// 		ft_fprintf(STDERR_FILENO, "minishell: empty command\n");
// 		exit(127);
// 	}
// 	cmd = find_path(node->cmd, data->env);
// 	if (ft_strchr(cmd, '/')) // Absolute/relative path
// 	{
// 		if (access(cmd, F_OK) != 0) // File doesn't exist
// 		{
// 			ft_fprintf(2, "%s: command not found\n", cmd);
// 			data->exit = 127;
// 			exit(127);
// 		}
// 		if (access(cmd, X_OK) != 0) // Not executable
// 		{
// 			ft_fprintf(2, "%s: Permission denied\n", cmd);
// 			data->exit = 126;
// 			exit(126);
// 		}
// 		if (is_directory(cmd)) // Is a directory
// 		{
// 			ft_fprintf(2, "%s: Is a directory\n", cmd);
// 			data->exit = 126;
// 			exit(126);
// 		}
// 	}
// 	else // Search in PATH
// 	{
// 		cmd = find_path(cmd, data->env);
// 		if (!cmd)
// 		{
// 			ft_fprintf(2, "%s: command not found\n", node->cmd);
// 			data->exit = 127;
// 			exit(127);
// 		}
// 	}
// 	// {
// 	// 	ft_fprintf(STDERR_FILENO, "%s: command not found\n", node->cmd);
// 	// 	exit(127);
// 	// }
// 	// if (is_directory(cmd))
// 	// {
// 	// 	ft_fprintf(STDERR_FILENO, "%s: is a directory\n", cmd);
// 	// 	exit(126);
// 	// }
// 	// if (access(cmd, X_OK) != 0)
// 	// {
// 	// 	ft_fprintf(STDERR_FILENO, "%s: Permission denied\n", cmd);
// 	// 	exit(126);
// 	// }
// 	env = list_to_str(data->env);
// 	if (execve(cmd, node->arg, env) == -1)
// 	{
// 		if (errno == EACCES)
// 		perror(node->cmd);
// 		exit(127);
// 	}
// }

/*
 * Checks if a command is a builtin that must execute in the parent process
 * Returns 1 for parent-builtins (cd, export, unset), 0 otherwise
 */
void execute_cmd(t_node *node, t_minishell *data)
{
    char *cmd;
    char **env;
    int result;

	// 3. Handle empty command
	if (!node->cmd || !node->cmd[0])
		exit(0);    // Get the command from the first argument

	node->cmd = node->arg[0];
		// Check if command is empty after variable expansion
	if (!node->cmd || !node->cmd[0])
	{
			int i = 0;
			while (node->arg[i + 1])
			{
				node->arg[i] = node->arg[i + 1];
				i++;
			}
			node->arg[i] = NULL;
			if (node->arg[0])
				node->cmd = node->arg[0];  // Update command

			else
				exit(0);
	}

    // 2. Handle regular builtins
    if (check_builtin(node->cmd))
    {
        handle_builtin(node->arg, data, &result);
        exit(result);
    }
    // 4. Handle path resolution
    if (!ft_strchr(node->cmd, '/')) // Only search PATH if not absolute/relative path
    {
        cmd = find_path(node->cmd, data->env);
        if (!cmd)
        {
            ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", node->cmd);
            exit(127);
        }
    }
	else
	{
		cmd = node->cmd;
		// 5. Check file existence and permissions
		if (access(cmd, F_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: No such file or directory\n", cmd);
			exit(127);
		}

		if (is_directory(cmd))
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: Is a directory\n", cmd);
			exit(126);
		}

		if (access(cmd, X_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: Permission denied\n", cmd);
			exit(126);
		}
	}
    // 6. Execute the command
    env = list_to_str(data->env);
    execve(cmd, node->arg, env);

    // If execve returns, there was an error
    ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
    exit(127);
}
