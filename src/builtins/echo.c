/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:40:44 by muhabin-          #+#    #+#             */
/*   Updated: 2025/04/15 20:12:25 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * @brief This function is to detect -n,tell echo not to print newline at EOL
 * @param args= array of string "hello" "world"
 * @param newline= to determine wheter a newline should be printed
 * @note if multiple -n flags it should treat it as one flags
*/
static int	skip_flags(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		printf("entered skip_flags\n");
		j = 1;
		if (args[i][j +1] != '\0' && args[i][j + 1] != 'n')
			break ;
		else if (args[i][j + 1] != '\0')
		{
			while (args[i][j + 1] == 'n')
				j++;
			if (args[i][j + 1] != 0)
				return (i);
		}
		*newline = 0;
		i++;
	}
	return (i);
}

/*
 @brief Used to count how many argument */
int	length(char **arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	while (arg[i])
		i++;
	return (i);
}

/*
 * @brief Replicate the behavior of the shell echo command
 * @param args an array of strings that argument pass to echo
 * @note Should be handling -n flag(suppresses newline at EOL)
 * @note length= calculates the number of argument in the arrays
 * @note skip_flags= process the -n flags and adjust newline variable
 * 	Example: Hello World
 * @return 0 in success
*/
int	ft_echo(char **args)
{
	int	newline;
	int	i;

	newline = 10;
	if (length(args) < 1)
	{
		printf("\n");
		return (0);
	}
	i = skip_flags(args, &newline);
	i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}
