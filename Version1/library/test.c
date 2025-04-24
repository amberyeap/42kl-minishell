void	execute_cmd(t_node *node, t_minishell *data)
 {
 	//check for expansion
 	char *cmd;
 	char **env;
 	char	*cmd;
 	char	**env;
 	int		result;

 	if (check_builtin(node->cmd))
 	{
 		handle_builtin(node->arg, data, &result);
 		exit(result);
 	}
 	cmd = expand_var(node->cmd, data->exit);
 	cmd = find_path(cmd, data->env);
 	if (cmd == NULL)
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
 		perror("Command not found");
 		data->exit = 126;
 		return ;
 		cmd = find_path(cmd, data->env);
 		if (cmd == NULL)
 		{
 			ft_fprintf(2, "%s: command not found\n", node->cmd);
 			data->exit = 127;
 			exit(127);
 		}
 	}
 	env = list_to_str(data->env);
 	// printf("debug: cmd = %s\n", cmd);
 	// for (int i = 0; node->arg[i]; i++)
  	// 	printf("debug: arg[%d] = %s\n", i, node->arg[i]);
 	if (execve(cmd, node->arg, env) == -1)
 	{
 		perror("Execution failed");
 		fprintf(stderr, "%s: command not found\n", node->cmd);
 		data->exit = 127;
 		exit(127);
 	}
 }
