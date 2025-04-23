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

// char	*join_words(char *word, char *chunk, char ***error_msg)
// {
// 	char	*temp;

// 	temp = ft_strjoin(word, chunk);
// 	if (!temp)
// 	{
// 		**error_msg = "failed to join words";
// 		return (NULL);
// 	}
// 	free(word);
// 	free(chunk);
// 	return (temp);
// }

char	*get_chunk(t_tokenizer *tokenizer, int *expand,
	char *quote, char ***error_msg)
{
	char	*chunk;

	if (is_quote(peek(tokenizer)))
	{
		*quote = peek(tokenizer);
		chunk = handle_quotes(tokenizer, *quote);
		if (!chunk)
			return (**error_msg = "unterminated quote", NULL);
		if (*quote == '"' && ft_strchr(chunk, '$'))
			*expand = 1;
	}
	else
	{
		chunk = handle_no_quotes(tokenizer);
		if (!chunk)
			return (**error_msg = "failed to get word", NULL);
		if (ft_strchr(chunk, '$'))
			*expand = 1;
	}
	return (chunk);
}

char	*get_word(t_tokenizer *tokenizer, int *expand, char **error_msg)
{
	char	*chunk;
	char	quote;
	char	*word;
	char	*temp;

	word = ft_strdup("");
	while (peek(tokenizer) && !is_whitespace(peek(tokenizer))
		&& !is_operator(peek(tokenizer)))
	{
		chunk = get_chunk(tokenizer, expand, &quote, &error_msg);
		if (!chunk)
			return (free(word), NULL);
		temp = ft_strjoin(word, chunk);
		if (!temp)
		{
			*error_msg = "failed to join words";
			return (free(word), free(chunk), NULL);
		}
		free(word);
		free(chunk);
		word = temp;
	}
	return (word);
}

t_token	*create_word_token(t_tokenizer *tokenizer)
{
	t_token	*new_token;
	char	*word;
	int		expand;
	char	*error_msg;

	expand = 0;
	word = get_word(tokenizer, &expand, &error_msg);
	if (!word)
		return (create_token(TOKEN_ERROR, error_msg));
	new_token = create_token(TOKEN_WORD, word);
	if (expand)
		new_token->expand = 1;
	free(word);
	return (new_token);
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
