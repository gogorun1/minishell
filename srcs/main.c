/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 22:28:45 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (EXIT_SUCCESS);
}

void	handle_sigint_in_main(t_shell *shell)
{
	if (g_signal_status == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal_status = 0;
	}
}

static int	check_terminal_modes(void)
{
	if (!isatty(STDOUT_FILENO))
	{
		printf("Redirection mode forbidden\n");
		exit(1);
	}
	if (!isatty(STDIN_FILENO))
	{
		printf("Non interactive mode forbidden\n");
		exit(1);
	}
	return (0);
}

static void	cleanup_main_loop(t_shell *shell)
{
	rl_clear_history();
	free_env(shell->env_list);
	clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		input_status;

	(void)argc;
	(void)argv;
	input_status = 0;
	check_terminal_modes();
	setup_signal_handlers();
	if (init_shell(&shell, envp) != 0)
		return (1);
	rl_event_hook = event;
	run_main_loop(&shell, input_status);
	cleanup_main_loop(&shell);
	return (shell.last_exit_status);
}
