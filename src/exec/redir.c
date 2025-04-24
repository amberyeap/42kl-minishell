/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:00:15 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/24 21:38:19 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int handle_input_redir(t_node *node, t_minishell *shell)
{
    int fd;

    fd = open(node->redir_file, O_RDONLY);
    // ft_fprintf(STDERR_FILENO, "Redirection applied: %d\n", fd);

    if (fd == -1) {
        ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
                  node->redir_file, strerror(errno));
        shell->exit = 1;
        return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static int handle_output_redir(t_node *node, t_minishell *shell)
{
    int fd;
    int flags = O_WRONLY | O_CREAT;

    flags |= (node->type == NODE_APPEND) ? O_APPEND : O_TRUNC;
    fd = open(node->redir_file, flags, 0644);
    if (fd == -1) {
        ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
                  node->redir_file, strerror(errno));
        shell->exit = 1;
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static int handle_heredoc(t_node *node, t_minishell *shell)
{
	int fd;
	fd = setup_heredoc(node, shell);
	printf("now should be EOF heredoc\n");
	if (fd == -1)
	{
		shell->exit = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return 0;
}

// int check_redir(t_node *node, t_minishell *shell)
// {
//     int stdin_fd = -1;
//     int stdout_fd = -1;
//     int result = 0;

//     while (node && result == 0) {

//         if (node->type == NODE_PIPE)
// 			break;
//         if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC) {
//             if (stdin_fd != -1) {
//                 close(stdin_fd);
//                 stdin_fd = -1;
//             }
//             if (node->type == NODE_REDIR_IN)
//                 result = handle_input_redir(node, shell);
//             else
//                 result = handle_heredoc(node, shell);
//         }
//         else if (node->type == NODE_REDIR_OUT || node->type == NODE_APPEND) {
//             if (stdout_fd != -1) {
//                 close(stdout_fd);
//                 stdout_fd = -1;
//             }
//             result = handle_output_redir(node, shell);
//         }
//         node = node->next;
//     }

//     // Cleanup if error occurred
//     if (result == -1) {
//         if (stdin_fd != -1) close(stdin_fd);
//         if (stdout_fd != -1) close(stdout_fd);
//     }
//     return result;
// }

// int check_redir(t_node *node, t_minishell *shell)
// {
//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     int result = 0;
//     t_node *current = node;

//     // First pass: process all redirections in order
//     while (current && result == 0)
//     {
//         if (current->type == NODE_PIPE)
//             break;
//         if (current->type == NODE_REDIR_IN)
//             result = handle_input_redir(current, shell);
//         else if (current->type == NODE_HEREDOC)
//             result = handle_heredoc(current, shell);
//         else if (current->type == NODE_REDIR_OUT || current->type == NODE_APPEND)
//             result = handle_output_redir(current, shell);

//         current = current->next;
//     }

//     // Cleanup if error occurred
//     if (result == -1) {
//         dup2(original_stdin, STDIN_FILENO);
//         dup2(original_stdout, STDOUT_FILENO);
//     }

//     close(original_stdin);
//     close(original_stdout);
//     return result;
// }

t_node *reverse_redir_list(t_node *start)
{
    t_node *prev = NULL;
    t_node *curr = start;
    t_node *next;

    while (curr && curr->type != NODE_PIPE)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}
// int check_redir(t_node *node, t_minishell *shell)
// {
//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     int result = 0;

//     // // reverse the redirection nodes
//     // t_node *reversed = reverse_redir_list(node);
//     t_node *current = node;
//     while (current && result == 0)
//     {
//         if (current->type == NODE_REDIR_IN)
//             result = handle_input_redir(current, shell);
//         else if (current->type == NODE_HEREDOC)
//             result = handle_heredoc(current, shell);
//         else if (current->type == NODE_REDIR_OUT || current->type == NODE_APPEND)
//             result = handle_output_redir(current, shell);

//         current = current->next;
//     }

//     // cleanup on error
//     if (result == -1)
//     {
//         dup2(original_stdin, STDIN_FILENO);
//         dup2(original_stdout, STDOUT_FILENO);
//     }

//     close(original_stdin);
//     close(original_stdout);
//     return result;
// }
int check_redir(t_node *node, t_minishell *shell) {
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int result = 0;

    // First pass: Handle HEREDOCs (they take precedence over regular input redirs)
    t_node *current = node;
    while (current && result == 0)
	{
        if (current->type == NODE_HEREDOC)
            result = handle_heredoc(current, shell);
        current = current->next;
    }

    // Second pass: Handle other redirections in reverse order (last one wins)
    if (result == 0) {
        current = node;
        t_node *last_input = NULL;
        t_node *last_output = NULL;

        // Find the last input and output redirections
        while (current) {
            if (current->type == NODE_REDIR_IN)
                last_input = current;
            else if (current->type == NODE_REDIR_OUT || current->type == NODE_APPEND)
                last_output = current;
            current = current->next;
        }

        // Apply the last input redirection (if any)
        if (last_input && result == 0)
            result = handle_input_redir(last_input, shell);

        // Apply the last output redirection (if any)
        if (last_output && result == 0)
            result = handle_output_redir(last_output, shell);
    }

    // Cleanup on error
    if (result == -1) {
        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);
    }

    close(original_stdin);
    close(original_stdout);
    return result;
}
