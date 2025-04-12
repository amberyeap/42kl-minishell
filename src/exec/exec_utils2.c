/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/12 10:08:45 by muhabin-         ###   ########.fr       */
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
		if (access(full_path, F_OK) == 0)
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
	char *cmd;
	char **env;

	cmd = expand_var(node->cmd);
	cmd = find_path(cmd, data->env);
	if (cmd == NULL)
	{
		perror("Command not found");
		data->exit = 127;
		return ;
	}
	env = list_to_str(data->env);
	if (execve(cmd, node->arg, env) == -1)
	{
		perror("Execution failed");
		data->exit = 126;
	}
}
