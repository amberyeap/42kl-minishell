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

#include "../../inc/minishell.h"

// creates a word token
/* 1. if the current char is a quote, it will call handle_quotes() 
** (34 is double quote, 39 is single quote)
*/
// t_token	*create_word_token(t_tokenizer *tokenizer)
// {
// 	t_token	*token;

// 	if (peek(tokenizer) == 34 || peek(tokenizer) == 39)
// 	{
// 		token = create_quote_token(tokenizer);
// 		if (token->type == TOKEN_ERROR)
// 			return (token);
// 		return (token);
// 	}
// 	else
// 	{
// 		token = create_unquoted_token(tokenizer);
// 		if (token->type == TOKEN_ERROR)
// 			return (token);
// 		return (token);
// 	}
// }

char *handle_no_quotes_until_quote_or_special(t_tokenizer *tokenizer)
{
	int start = tokenizer->position;
	int len;
	char *word;

	while (peek(tokenizer) &&
		   !is_whitespace(peek(tokenizer)) &&
		   !is_operator(peek(tokenizer)) &&
		   peek(tokenizer) != '\'' &&
		   peek(tokenizer) != '"')
	{
		next_char(tokenizer);
	}

	len = tokenizer->position - start;
	word = ft_substr(tokenizer->user_input, start, len);
	return word;
}


t_token	*create_word_token(t_tokenizer *tokenizer)
{
	t_token *token;
	char *chunk;
	char *word = ft_strdup("");  // Start with an empty string
	int expand = 0;

	if (!word)
		return (create_token(TOKEN_ERROR, "Memory allocation failed"));

	while (peek(tokenizer) &&
		   !is_whitespace(peek(tokenizer)) &&
		   !is_operator(peek(tokenizer)))
	{
		if (peek(tokenizer) == 34 || peek(tokenizer) == 39)  // '"' or '\''
		{
			char quote = peek(tokenizer);
			chunk = handle_quotes(tokenizer, quote);
			if (!chunk)
			{
				free(word);
				return (create_token(TOKEN_ERROR, "Unterminated quote"));
			}
			if (quote == 34 && ft_strchr(chunk, '$')) // double quote allows var expansion
				expand = 1;
		}
		else
		{
			chunk = handle_no_quotes_until_quote_or_special(tokenizer);
			if (!chunk)
			{
				free(word);
				return (create_token(TOKEN_ERROR, "Failed to create word token"));
			}
			if (ft_strchr(chunk, '$'))
				expand = 1;
		}

		char *tmp = ft_strjoin(word, chunk);
		free(word);
		free(chunk);
		if (!tmp)
			return (create_token(TOKEN_ERROR, "Memory allocation failed"));
		word = tmp;
	}

	token = create_token(TOKEN_WORD, word);
	if (expand)
		token->expand = 1;
	free(word);  // Assume create_token makes a copy of the word
	return token;
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
