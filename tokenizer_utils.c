/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:37:31 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/18 18:37:31 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get current character in the user_input
char	peek(t_tokenizer *tokenizer)
{
	return (tokenizer->user_input[tokenizer->position]);
}

// moves to the next character in the user_input
void	next_char(t_tokenizer *tokenizer)
{
	tokenizer->position++;
}

// skips whitespace
void	skip_whitespace(t_tokenizer *tokenizer)
{
	while (peek(tokenizer) && is_whitespace(peek(tokenizer)))
		next_char(tokenizer);
}

// initializes the tokenizer
t_tokenizer	*init_tokenizer(char *input)
{
	t_tokenizer	*tokenizer;

	tokenizer = malloc(sizeof(t_tokenizer));
	if (!tokenizer)
		return (NULL);
	tokenizer->user_input = input;
	tokenizer->position = 0;
	// tokenizer->error = 0;
	// tokenizer->error_msg = NULL;
	return (tokenizer);
}

void	free_token_list(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
