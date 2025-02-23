#include "minishell.h"

int main() {
   // name of the environment
	const char*	env_variable = "HOME";
   // Retrieve the value
	char* value = getenv(env_variable); 

	if (value != NULL) {
		printf("Variable = %s \nValue %s", env_variable, value);
	} else {
		printf("Environment Variable doesn't exist!");
	}
	return 0;
}

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