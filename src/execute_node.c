# include "minishell.h"

void execute_node(t_minishell *shell)
{
	t_node *current_node;

	if (current_node->type == NODE_PIPE)
		setup_pipe();
	else if (current_node->type == NODE_REDIR_IN)
		setup_redir_in();
	else if (current_node->type == NODE_REDIR_OUT)
		setup_redir_out();
	else if (current_node->type == NODE_HEREDOC)
		setup_heredoc();
	else if (current_node->type == NODE_APPEND)
		setup_append();
}