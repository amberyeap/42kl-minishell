/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:00:15 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/25 20:59:03 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int handle_input_redir(t_node *node, t_minishell *shell)
{
    int fd;

    fd = open(node->redir_file, O_RDONLY);
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

// int check_redir(t_node *node, t_minishell *shell) {
//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     int result = 0;

//     // First pass: Handle HEREDOCs (they take precedence over regular input redirs)
//     t_node *current = node;
//     while (current && result == 0)
// 	{
//         if (current->type == NODE_HEREDOC)
//             result = handle_heredoc(current, shell);
//         current = current->next;
//     }

//     // Second pass: Handle other redirections in reverse order (last one wins)
//     if (result == 0) {
//         current = node;
//         t_node *last_input = NULL;
//         t_node *last_output = NULL;

//         // Find the last input and output redirections
//         while (current) {
//             if (current->type == NODE_REDIR_IN)
//                 last_input = current;
//             else if (current->type == NODE_REDIR_OUT || current->type == NODE_APPEND)
//                 last_output = current;
//             current = current->next;
//         }

//         // Apply the last input redirection (if any)
//         if (last_input && result == 0)
//             result = handle_input_redir(last_input, shell);

//         // Apply the last output redirection (if any)
//         if (last_output && result == 0)
//             result = handle_output_redir(last_output, shell);
//     }

//     // Cleanup on error
//     if (result == -1) {
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
    t_node *last_input = NULL;
    t_node *last_output = NULL;
    t_node *last_heredoc = NULL;

    // First pass: Find last redirections and verify input files exist
    t_node *current = node;
    while (current) {
        if (current->type == NODE_HEREDOC) {
            last_heredoc = current;
        }
        else if (current->type == NODE_REDIR_IN) {
            // Verify file exists immediately
            if (access(current->redir_file, F_OK) == -1) {
                ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
                          current->redir_file, strerror(errno));
                result = -1;
                break;
            }
            last_input = current;
        }
        else if (current->type == NODE_REDIR_OUT || current->type == NODE_APPEND) {
            last_output = current;
        }
        current = current->next;
    }

    // Only proceed if all input files exist
    if (result == 0) {
        // Apply heredoc first (highest precedence)
        if (last_heredoc)
            result = handle_heredoc(last_heredoc, shell);

        // Then apply regular input redirection
        if (result == 0 && last_input)
            result = handle_input_redir(last_input, shell);

        // Finally apply output redirection
        if (result == 0 && last_output)
            result = handle_output_redir(last_output, shell);
    }

    // Cleanup on error
    if (result == -1) {
        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);
        shell->exit = 1;
    }

    close(original_stdin);
    close(original_stdout);
    return result;
}
