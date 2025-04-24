/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:00:15 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/23 00:50:42 by muhabin-         ###   ########.fr       */
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
    int fd = setup_heredoc(node, shell);

    if (fd == -1) {
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

int check_redir(t_node *node, t_minishell *shell)
{
    int stdin_fd = -1;
    int stdout_fd = -1;
    int result = 0;

    while (node && result == 0) {

        if (node->type == NODE_PIPE)
			break;
        if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC) {
            if (stdin_fd != -1) {
                close(stdin_fd);
                stdin_fd = -1;
            }
            if (node->type == NODE_REDIR_IN)
                result = handle_input_redir(node, shell);
            else
                result = handle_heredoc(node, shell);
        }
        else if (node->type == NODE_REDIR_OUT || node->type == NODE_APPEND) {
            if (stdout_fd != -1) {
                close(stdout_fd);
                stdout_fd = -1;
            }
            result = handle_output_redir(node, shell);
        }
        node = node->next;
    }

    // Cleanup if error occurred
    if (result == -1) {
        if (stdin_fd != -1) close(stdin_fd);
        if (stdout_fd != -1) close(stdout_fd);
    }
    return result;
}
