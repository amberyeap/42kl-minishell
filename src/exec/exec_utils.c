/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:31:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 13:02:04 by muhabin-         ###   ########.fr       */
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
	printf("right now inside set_child\n");
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(1);
		}
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out_fd");
			exit(1);
		}
		close(out_fd);
	}
	execute_cmd(cmd, data);
	exit(data->exit);
}
// static void	create_pipe(int pipefd[2])
// {
// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		exit(1);
// 	}
// 	printf("[DEBUG] Pipe created: [%d, %d]\n", pipefd[0], pipefd[1]);
// }

// void	set_pipe(t_node *current_node, t_minishell *data)
// {
// 	int		pipefd[2];
// 	int		input_fd;
// 	pid_t	pid;

// 	input_fd = STDIN_FILENO;
// 	while (current_node)
// 	{
// 		if (current_node->next && current_node->next->type == NODE_PIPE)
// 			pipe(pipefd);
// 			// create_pipe(pipefd);
// 		else
// 			pipefd[1] = STDOUT_FILENO;
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			exit(1);
// 		}
// 		if (pid == 0)
// 		{
// 			if (input_fd != STDIN_FILENO)
// 			{
// 				dup2(input_fd, STDIN_FILENO);
// 				close(input_fd);
// 			}
// 			if (pipefd[1] != STDOUT_FILENO)
// 			{
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				close(pipefd[1]);
// 			}
// 			close(pipefd[0]);
// 			set_child(input_fd, pipefd[1], data, current_node);
// 		}
// 		if (input_fd != STDIN_FILENO)
// 			close(input_fd); // Close previous read-end
// 		input_fd = pipefd[0];
// 		current_node = current_node->next;
// 		if (current_node && current_node->type == NODE_PIPE)
// 			current_node = current_node->next;
// 	}
// 	while(wait(NULL) > 0);
// 	printf("[DEBUG] Execution finished.\n");
// }
/*
void set_pipe(t_node *pipeline, t_minishell *shell)
{
    int pipefd[2];
    int input_fd = STDIN_FILENO; // Start with stdin
    pid_t pid;
	pid_t	last_pid;
	int has_pipe;
	int	status;

    while (pipeline)
    {
		has_pipe = 0;
        if (pipeline->next && pipeline->next->type == NODE_PIPE)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(1);
            }
			has_pipe = 1;
        }
        else
        {
            pipefd[1] = STDOUT_FILENO; // Last command writes to stdout
			pipefd[0] = -1;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0) // Child process
        {
            if (input_fd != STDIN_FILENO)
            {
                if (dup2(input_fd, STDIN_FILENO) == -1);
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
                close(input_fd);
            }
			if (has_pipe)
			{
				dup2(pipefd[1], STDOUT_FILENO);
			}
			if (has_pipe || pipefd[1] != STDOUT_FILENO)
				close(pipefd[1]);
			if (pipefd[0] != -1)
				close(pipefd[0]);
            execute_cmd(pipeline, shell);
            exit(shell->exit);
        }

        // Parent process
		if (input_fd != STDIN_FILENO)
			close(input_fd); // Close previous read-end
		if (has_pipe)
		{
			close(pipefd[1]); // Close write-end of the pipe in parent
			input_fd = pipefd[0]; // Set input_fd to read-end of the pipe
		}
		else
			input_fd = STDIN_FILENO;
		// if (has_pipe)
        // 	close(pipefd[1]);
        // input_fd = pipefd[0];
        pipeline = pipeline->next;
        if (pipeline && pipeline->type == NODE_PIPE)
            pipeline = pipeline->next;
    }

    // Wait for all child processes
	while ((pid = wait(&status)) > 0) {
		if (pid == last_pid) {
			shell->exit = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
		}
	}
}
*/
void	set_pipe(t_node *node, t_minishell *data)
{
	int		pipe_fd[2];
	int		prev_pipe_in = -1;
	pid_t	pid;
	t_node	*current = node;

	while (current != NULL)
	{
		if (current->next != NULL && current->next->type == NODE_PIPE)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell: pipe");
				data->exit = EXIT_FAILURE;
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			data->exit = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_pipe_in != -1)
			{
				if (dup2(prev_pipe_in, STDIN_FILENO) == -1)
				{
					perror("minishell: dup2");
					data->exit = EXIT_FAILURE;
					exit(EXIT_FAILURE);
				}
				close(prev_pipe_in);
			}
			if (current->next != NULL && current->next->type == NODE_PIPE)
			{
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				{
					perror("minishell: dup2");
					exit(EXIT_FAILURE);
				}
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			execute_cmd(current, data);
			exit(data->exit);
		}
		else
		{
			if (prev_pipe_in != -1)
				close(prev_pipe_in);
			if (current->next != NULL && current->next->type == NODE_PIPE)
			{
				prev_pipe_in = pipe_fd[0];
				close(pipe_fd[1]);
			}
			else
				prev_pipe_in = -1;
			current = current->next;
			if (current != NULL && current->type == NODE_PIPE)
				current = current->next;
		}
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}
