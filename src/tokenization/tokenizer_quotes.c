/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:20:48 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 21:21:12 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_quote_token(t_tokenizer *tokenizer)
{
	char	*word;
	t_token	*token;
	char	quote;

	quote = peek(tokenizer);
	word = handle_quotes(tokenizer, quote);
	if (!word)
		return (create_token(TOKEN_ERROR, "Unterminated quote"));
	token = create_token(TOKEN_WORD, word);
	if (quote == 34)
		token->quote = DOUBLE_QUOTE;
	else
		token->quote = SINGLE_QUOTE;
	if (ft_strchr(word, '$') && token->quote == DOUBLE_QUOTE)
		token->expand = 1;
	free(word);
	return (token);
}

char	*handle_quotes(t_tokenizer *tokenizer, char quote)
{
	int		start;
	int		len;
	char	*word;

	next_char(tokenizer);
	start = tokenizer->position;
	while (peek(tokenizer) != quote && peek(tokenizer))
		next_char(tokenizer);
	len = tokenizer->position - start;
	if (peek(tokenizer) != quote)
		return (NULL);
	word = ft_substr(tokenizer->user_input, start, len);
	next_char(tokenizer);
	return (word);
}
