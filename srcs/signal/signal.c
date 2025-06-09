#include "minishell.h"

void signal_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void setup_signal_handlers(void)
{
	rl_catch_signals = 0; // Disable readline's default signal handling
	
	struct sigaction sa_int;
	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART | SA_SIGINFO; // Restart syscalls and provide siginfo
	sigaction(SIGINT, &sa_int, NULL);   // Ctrl-C
	signal(SIGQUIT, SIG_IGN); // Ctrl-\ ignore SIGQUIT signal
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);   // Ctrl-C
	signal(SIGQUIT, SIG_DFL); // Ctrl-\ reset to default handler
}