/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/22 21:06:19 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_path(char *cmd, t_env *env)
{
	char	*path;
	char	*full_path;
	char	**paths;
	int		i;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL);
}

void	execute_cmd(t_node *node, t_minishell *data)
{
	//check for expansion
	char	*cmd;
	char	**env;
	int		result;

	if (check_builtin(node->cmd))
	{
		handle_builtin(node->arg, data, &result);
		exit(result);
	}
	cmd = expand_var(node->cmd, data->exit);
	if (cmd == NULL || cmd[0] == '\0')
	{
		data->exit = 0;
		exit(0);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0) //Files not exit
		{
			fprintf(stderr, "%s: command not found\n", cmd);
			data->exit = 127;
			exit(127);
		}
		if (access(cmd, X_OK) != 0) // not executable
		{
			fprintf(stderr, "%s: Permission denied\n", cmd);
			data->exit = 126;
			exit(126);
		}
		if (is_directory(cmd))
		{
			ft_fprintf(2, "%s: Is a directory\n", cmd);
			data->exit = 126;
			exit(126);
		}
	}
	else
	{
		cmd = find_path(cmd, data->env);
		if (cmd == NULL)
		{
			ft_fprintf(2, "%s: command not found\n", node->cmd);
			data->exit = 127;
			exit(127);
		}
	}
	env = list_to_str(data->env);
	if (execve(cmd, node->arg, env) == -1)
	{
		fprintf(stderr, "%s: command not found\n", node->cmd);
		data->exit = 127;
		exit(127);
	}
}
