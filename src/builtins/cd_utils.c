/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:01:07 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 20:06:24 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
*	* it get the input then check is the key is found in env
*	* then is found return the value of the key found
*	* Example(env, "HOME"): it search inside env for HOME then
		if found it should return the value(after=)
		return NULL means not found
*/
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
	* Update the OLDPWD
	1) First using getcwd to get the current working directory
	2) Then update the env_var of OLDPWD with current directory
*/
void	update_oldpwd(t_minishell *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
		update_env_var(data, "OLDPWD", cwd);
}

/*
	* Update the Current Working Directory
	1) Same concept but instead Update PWD
*/
void	update_pwd(t_minishell *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
		update_env_var(data, "PWD", cwd);
}

int	change_directory(char *path)
{
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "Minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}
