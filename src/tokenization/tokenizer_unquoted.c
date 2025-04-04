/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_unquoted.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:21:23 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/23 21:22:39 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_no_quotes(t_tokenizer *tokenizer)
{
	int		start;
	int		len;
	char	*word;

	start = tokenizer->position;
	while (!is_whitespace(peek(tokenizer))
		&& !is_operator(peek(tokenizer)) && peek(tokenizer))
		next_char(tokenizer);
	len = tokenizer->position - start;
	word = ft_substr(tokenizer->user_input, start, len);
	return (word);
}

t_token	*create_unquoted_token(t_tokenizer *tokenizer)
{
	char	*word;
	t_token	*token;

	word = handle_no_quotes(tokenizer);
	if (!word)
		return (create_token(TOKEN_ERROR, "Failed to create word token"));
	token = create_token(TOKEN_WORD, word);
	if (ft_strchr(word, '$'))
		token->expand = 1;
	free(word);
	return (token);
}
