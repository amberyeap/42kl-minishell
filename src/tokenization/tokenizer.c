/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:37:53 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 21:22:34 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// creates a word token
/* 1. if the current char is a quote, it will call handle_quotes() 
** (34 is double quote, 39 is single quote)
*/
t_token	*create_word_token(t_tokenizer *tokenizer)
{
	t_token	*token;

	if (peek(tokenizer) == 34 || peek(tokenizer) == 39)
	{
		token = create_quote_token(tokenizer);
		if (token->type == TOKEN_ERROR)
			return (token);
		return (token);
	}
	else
	{
		token = create_unquoted_token(tokenizer);
		if (token->type == TOKEN_ERROR)
			return (token);
		return (token);
	}
}

// creates an operator token
t_token	*create_op_token(t_tokenizer *tokenizer)
{
	char	op;

	op = peek(tokenizer);
	next_char(tokenizer);
	if (op == '|')
		return (create_token(TOKEN_PIPE, NULL));
	else if (op == '<' || op == '>')
	{
		if (peek(tokenizer) == op)
		{
			next_char(tokenizer);
			if (op == '<')
				return (create_token(TOKEN_HEREDOC, NULL));
			return (create_token(TOKEN_APPEND, NULL));
		}
		else
		{
			if (op == '<')
				return (create_token(TOKEN_REDIR_IN, NULL));
			return (create_token(TOKEN_REDIR_OUT, NULL));
		}
	}
	return (create_token(TOKEN_ERROR, "Unrecognized operator"));
}

// main tokenize function
// 1. skips whitespace
// 2. checks the char in the user_input.
//	- if it's a nul terminator, 
//		it will break out of the loop and create a EOF token
//	- if it's an operator (|<>), it will create an operator token
//	- else if it's a word, it will create a word token 
// 3. check if there was any error encountered
//	- if NO error, create EOF token
//	- if there is error, free all malloc and set the head pointer to NULL 
// 4. free tokenizer as the user_input has been tokenized
// 5. return the head pointer, which is a pointer to a list of tokens
t_token	*tokenize(char *input)
{
	t_tokenizer	*tokenizer;
	t_token		*head;
	t_token		*new_token;

	head = NULL;
	tokenizer = init_tokenizer(input);
	while (peek(tokenizer))
	{
		skip_whitespace(tokenizer);
		if (peek(tokenizer) == '\0')
			break ;
		if (is_operator(peek(tokenizer)))
			new_token = create_op_token(tokenizer);
		else
			new_token = create_word_token(tokenizer);
		add_token(&head, new_token);
	}
	free(tokenizer);
	return (head);
}
