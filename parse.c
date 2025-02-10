#include "minishell.h"

t_ast *parse_redirection(t_parser *parser)
{
	t_ast *redir_node;
	t_token_type redir_type;

	redir_type = parser->current_token->type;
	consume_token(parser);
	
}

// prefixes are IO_REDIR and ASSIGNMENT_WORD ('=')
t_ast *parse_prefix(t_parser *parser)
{
	if (is_redir_token(parser->current_token->type))
		parse_redirection(parser);
}

t_ast *parse_simple_command(t_parser *parser)
{
	t_ast *prefix_node = NULL;
	t_ast *cmd_node = NULL;
	t_ast *suffix_node = NULL;

	prefix_node = parse_prefix(parser);
	cmd_node = parse_command(parser);
	suffix_node = parse_suffix(parser);

}

t_ast *parse_pipeline(t_parser *parser)
{
	t_ast *left;
	t_ast *pipe_node;

	left = parse_simple_command(parser);
}

t_ast *parse(t_parser *parser, t_token *token_list)
{
	t_ast *root;

	if (!token_list)
		return (NULL);
	root = parse_pipeline(parser);
	if (!root)
		return (NULL);
}