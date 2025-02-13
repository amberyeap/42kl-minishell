#include "minishell.h"

char *get_user_input()
{
	char *line;
	line = readline("minishell$ ");
	if (line && *line)
		add_history(line);
	return (line);
}

int main()
{
	while (1)
		get_user_input();
	return (0);
}