/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:43:03 by ayeap             #+#    #+#             */
/*   Updated: 2024/11/28 14:43:03 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




int main(int argc, char **argv) {
    // if (argc > 1) {
    //     printf("Welcome, %s!\n", argv[1]);
    // } else {
    //     printf("Welcome, user!\n");
    // }
	(void)argc;
	(void)argv;

    while (1) {
        char *line = readline("minishell$ ");
        if (!line || strcmp(line, "exit") == 0) {  // Exit on EOF or "exit"
            free(line);
            break;
        }
        // printf("You entered: %s\n", line);
        free(line);
    }

    return 0;
}
