// #include "minishell.h"

#include "library/inc/libft.h"
#include <string.h>

// int main() {
//    // name of the environment
// 	const char*	env_variable = "HOME";
//    // Retrieve the value
// 	char* value = getenv(env_variable); 

// 	if (value != NULL) {
// 		printf("Variable = %s \nValue %s", env_variable, value);
// 	} else {
// 		printf("Environment Variable doesn't exist!");
// 	}
// 	return 0;
// }

// char *get_user_input()
// {
// 	char *line;
// 	line = readline("minishell$ ");
// 	if (line && *line)
// 		add_history(line);
// 	return (line);
// }

// int main()
// {
// 	while (1)
// 		get_user_input();
// 	return (0);
// }

typedef struct s_data {

	char **split_path;

} t_data;

void look_for_cmd(t_data *data, char *cmd)
{
	char *temp;
	char *full_path;
	int i = 0;

	while (data->split_path[i])
	{
		temp = ft_strjoin(data->split_path[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);

		// printf("%s\n", full_path);

		if (access(full_path, F_OK | X_OK) == 0)
		{
			char **temp2 = ft_split(cmd, ' ');
			execve(full_path, temp2, NULL);
			break;
		}
		i++;
	}
}

// search environment variables for PATH
// split the PATH into multiple strings by the ":"
void get_split_env(char **env, t_data *data)
{
	int i = 0;
	char *env_copy;

	while (env[i]) {
		if (strncmp("PATH=", env[i], 5) == 0)
			env_copy = env[i];
		i++;
	}
	data->split_path = ft_split(env_copy + 5, ':');
}

int main(int argc, char **argv, char **env)
{
	t_data data;

	(void)argc;
	// parse_user_input(argv, &data);
	get_split_env(env, &data);

	// to test if get_split_env() works as intended
	// int i = 0;
	// while (data.split_path[i])
	// 	printf("%s\n", data.split_path[i++]);

	return 0;
}