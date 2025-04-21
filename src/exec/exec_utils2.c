/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:13:53 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/17 19:29:37 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	free(paths);
	return (NULL);
}

void	execute_cmd(t_node *node, t_minishell *data)
{
	//check for expansion
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
	if (cmd_path == NULL)
	{
		perror("Command not found");
		data->exit = 126;
		return ;
	}
	env = list_to_str(data->env);
	// printf("debug: cmd = %s\n", cmd);
	// for (int i = 0; node->arg[i]; i++)
 	// 	printf("debug: arg[%d] = %s\n", i, node->arg[i]);
	if (execve(cmd_path, node->arg, env) == -1)
	{
		perror("Execution failed");
		data->exit = 127;
	}
}
