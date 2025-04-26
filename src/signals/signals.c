/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:33:12 by ayeap             #+#    #+#             */
/*   Updated: 2025/02/19 22:33:12 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	new_prompt(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = new_prompt;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
}

void	check_signal(int status, t_minishell *data)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		data->exit = 128 + sig;
		if (sig == SIGINT)
			printf("\n");
		if (sig == SIGQUIT)
			printf("Quit (core dumped)\n");
	}
}
