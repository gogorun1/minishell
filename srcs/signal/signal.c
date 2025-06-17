/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:32 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 22:02:54 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	interactive_sigint_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGINT)
	{
		rl_done = 1;
		rl_replace_line("", 0);
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;

	rl_catch_signals = 0;
	sa_int.sa_handler = interactive_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Signal handler during command execution
void	execution_sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_execution_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
