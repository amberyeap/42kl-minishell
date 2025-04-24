/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/24 10:32:29 by muhabin-         ###   ########.fr       */
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

void	execute_cmd(t_node *node, t_minishell *data)
{
	char	*cmd_expanded;
	char	*cmd_path;
	char	**env;

	if (!node->arg || !node->arg[0])
	{
		ft_fprintf(STDERR_FILENO, "minishell: empty command\n");
		data->exit = 127;
		return ;
	}
	cmd_expanded = expand_var(node->cmd, data->exit);
	cmd_path = find_path(cmd_expanded, data->env);
	free(cmd_expanded);
	if (cmd_path == NULL)
	{
		perror("Command not found");
		data->exit = 126;
		return ;
	}
	env = list_to_str(data->env);
	if (execve(cmd_path, node->arg, env) == -1)
	{
		fprintf(stderr, "%s: command not found\n", node->cmd);
		data->exit = 127;
		exit(127);
	}
}
