/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/26 10:47:45 by muhabin-         ###   ########.fr       */
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

void	empty_cmd(t_node *node)
{
	int	i;

	if (!node->cmd || !node->cmd[0])
	{
		i = 0;
		while (node->arg[i + 1])
		{
			node->arg[i] = node->arg[i + 1];
			i++;
		}
		node->arg[i] = NULL;
		if (node->arg[0])
			node->cmd = node->arg[0];
		else
			exit(0);
	}
}

char	*cmd_path(t_node *node, t_minishell *data)
{
	char	*cmd;

	if (!ft_strchr(node->cmd, '/'))
	{
		cmd = find_path(node->cmd, data->env);
		if (!cmd)
			print_and_exit_error("command not found", node->cmd, 127);
	}
	else
	{
		cmd = node->cmd;
		if (access(cmd, F_OK) != 0)
			print_and_exit_error("No such file or directory", cmd, 127);
		if (is_directory(cmd))
			print_and_exit_error("Is a directory", cmd, 126);
		if (access(cmd, X_OK) != 0)
			print_and_exit_error("Permission denied", cmd, 126);
	}
	return (cmd);
}

void	execute_cmd(t_node *node, t_minishell *data)
{
	char	*cmd;
	char	**env;

	if (!node->cmd || !node->cmd[0])
		exit(0);
	node->cmd = node->arg[0];
	empty_cmd(node);
	cmd = cmd_path(node, data);
	env = list_to_str(data->env);
	execve(cmd, node->arg, env);
	ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
	exit(127);
}
