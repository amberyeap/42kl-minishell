/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:31:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/24 23:22:32 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void set_pipe(t_node *node, t_minishell *data)
{
    int pipe_fd[2];
    int prev_pipe_in = -1;
    pid_t last_pid = -1;
	pid_t	pid;
    t_node *current = node;

    while (current)
    {
        int has_pipe = current->next && current->next->type == NODE_PIPE;

        if (has_pipe && pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            data->exit = EXIT_FAILURE;
            return;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            data->exit = EXIT_FAILURE;
            return ;
        }
        else if (pid == 0) // === CHILD ===
        {
            // 1. If input comes from previous pipe
            if (prev_pipe_in != -1)
            {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }

            // 2. If output goes to next command (pipe)
            if (has_pipe)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            // 3. Redirections should override above dup2 if needed
            if (check_redir(current, data) == -1)
                exit(1);

            // 4. Execute command
            execute_cmd(current, data);
            exit(data->exit);
        }
        else // === PARENT ===
        {
            last_pid = pid;

            // Clean up previous input pipe (we're done with it)
            if (prev_pipe_in != -1)
                close(prev_pipe_in);

            // Prepare input pipe for next iteration
            prev_pipe_in = has_pipe ? pipe_fd[0] : -1;

            // Close write end in parent (only reader child needs it)
            if (has_pipe)
                close(pipe_fd[1]);

            // Skip pipe node if present
            current = has_pipe ? current->next->next : current->next;
        }
    }

    // === Wait for children ===
    int status;
    while (waitpid(-1, &status, 0) > 0)
        if (WIFEXITED(status) && pid == last_pid)
            data->exit = WEXITSTATUS(status);
}
