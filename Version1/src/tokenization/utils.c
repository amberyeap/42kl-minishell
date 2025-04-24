/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:36:21 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/18 18:36:21 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// returns a number (aka true) if the char is a whitespace
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// returns a number (aka true) if the char is an operator
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// returns a number (aka true) if the char is a quote
int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}
