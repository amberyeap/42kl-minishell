/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:31:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/04 19:27:35 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
// 	* close the write end(not needed in parent)
// 	* redirect the output to stdout
// 	* close the dup

// */
// void	parent_process(t_node *current_node, int fd[2])
// {
// 	close(fd[1]);
// 	dup2(fd[0], STDIN_FILENO);
// 	close(fd[0]);
// 	exec_cmd(current_node->next);
// }
// /*
// 	* close the read end(not needed in child)
// 	* redirect the output to the write end of pipe
// 	* close the after dup
// 	* runs the command
// */
// void	child_process(t_node *current_node, int fd[2])
// {
// 	close(fd[0]);
// 	dup2(fd[1],STDOUT_FILENO);
// 	close(fd[1]);
// 	exec_cmd(current_node);
// 	perror("exec_cmd");
// 	exit(1);

// }
// //setting up pipe
// void	setup_pipe(t_node *current_node)
// {
// 	int	fd[2];
// 	pid_t	pid;

// 	if (pipe(fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(1);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(1);
// 	}
// 	if (pid == 0) //chilp process
// 		child_process(current_node, fd);
// 	waitpid(pid, NULL, 0);
// 	parent_process(current_node, fd);
// }

//
// void	closed_fd(int *in_fd, int out_fd)
// {
// 	if (*in_fd != STDIN_FILENO)
// 		close(*in_fd);
// 	if (out_fd != STDOUT_FILENO)
// 		close(out_fd);
// 	*in_fd = out_fd;
// }
void	set_child(int in_fd, int out_fd, t_minishell *data, t_node *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	execute_fork(cmd, data);
	exit(data->exit);
}
static void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	printf("[DEBUG] Pipe created: [%d, %d]\n", pipefd[0], pipefd[1]);
}
void	set_pipe(t_node *current_node, t_minishell *data)
{
	int		pipefd[2];
	int		input_fd;
	pid_t	pid;

	input_fd = STDIN_FILENO; //start with stdin, takes from the 1 pipes
	while (current_node)
	{
		if (current_node->next && current_node->next->type == NODE_PIPE) //Check if the next node is a pipe
			create_pipe(pipefd);
		else
			pipefd[1] = STDOUT_FILENO; //Last cmd writes to stdout
		pid = fork();
		if (pid == 0)
			set_child(input_fd, pipefd[1], data, current_node);
		printf("[DEBUG] Parent PID %d created child PID %d for command: %s\n",
				getpid(), pid, current_node->arg[0]); // Add this for clarity

		// closed_fd(&input_fd, pipefd[0]);
		close(pipefd[1]);
		if (input_fd != STDIN_FILENO)
			close(input_fd); // Close previous read-end
		// Update input_fd for the next command
		input_fd = pipefd[0];
		current_node = current_node->next;
		if (current_node && current_node->type == NODE_PIPE)
			current_node = current_node->next;
	}
	while(wait(NULL) > 0);
	printf("[DEBUG] Execution finished.\n");
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// Define node types
// #define NODE_COMMAND 1
// #define NODE_PIPE 2

// // Function prototype from your shell code
// void set_pipe(t_node *current_node, t_minishell *data);

// // Helper function to create a command node
// t_node *create_command(char **args)
// {
//     t_node *new_node = malloc(sizeof(t_node));
//     new_node->type = NODE_COMMAND;
//     new_node->arg = args;
//     new_node->next = NULL;
//     return new_node;
// }

// // Helper function to create a pipe node
// t_node *create_pipe_node()
// {
//     t_node *new_node = malloc(sizeof(t_node));
//     new_node->type = NODE_PIPE;
//     new_node->arg = NULL;
//     new_node->next = NULL;
//     return new_node;
// }

// // Dummy execute_fork function to mimic command execution
// void execute_fork(t_minishell *data, t_node *cmd)
// {
// 	(void)data;
//     execvp(cmd->arg[0], cmd->arg);
//     perror("execvp failed");
//     exit(1);
// }


// // Main function to test the pipeline execution
// int main()
// {
//     t_minishell shell;
//     shell.exit = 0;

//     // Test command: ls | grep "test" | wc -l
//     char *cmd1[] = {"/bin/ls", NULL};
//     char *cmd2[] = {"/usr/bin/grep", "test", NULL};
//     char *cmd3[] = {"/usr/bin/wc", "-l", NULL};

//     // Creating nodes
//     t_node *cmd_node1 = create_command(cmd1);
//     t_node *pipe_node1 = create_pipe_node();
//     t_node *cmd_node2 = create_command(cmd2);
//     t_node *pipe_node2 = create_pipe_node();
//     t_node *cmd_node3 = create_command(cmd3);

//     // Linking the nodes: ls | grep "test" | wc -l
//     cmd_node1->next = pipe_node1;
//     pipe_node1->next = cmd_node2;
//     cmd_node2->next = pipe_node2;
//     pipe_node2->next = cmd_node3;

//     // Execute the pipeline
//     set_pipe(cmd_node1, &shell);

//     return 0;
// }
