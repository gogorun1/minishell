#include "minishell.h"

// void signal_handler(int sig)
// {
// 	g_signal_status = sig;
// 	if (sig == SIGINT)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

// void setup_signal_handlers(void)
// {
// 	rl_catch_signals = 0; // Disable readline's default signal handling
	
// 	struct sigaction sa_int;
// 	sa_int.sa_handler = signal_handler;
// 	sigemptyset(&sa_int.sa_mask);
// 	sa_int.sa_flags = SA_RESTART | SA_SIGINFO; // Restart syscalls and provide siginfo
// 	sigaction(SIGINT, &sa_int, NULL);   // Ctrl-C
// 	signal(SIGQUIT, SIG_IGN); // Ctrl-\ ignore SIGQUIT signal
// }

// void	setup_child_signals(void)
// {
// 	signal(SIGINT, SIG_DFL);   // Ctrl-C
// 	signal(SIGQUIT, SIG_DFL); // Ctrl-\ reset to default handler
// }

int	readline_function(void)
{
	return (0);
}

void	set_parent_signals(void)
{
	rl_event_hook = readline_function;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	set_child_signals(void)
{
	rl_event_hook = readline_function;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_IGN);
}

void	signal_handler(int signum)
{
	g_signal_status = 128 + signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		rl_done = 1;
	}
}

void	heredoc_sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_status = 128 + signum;
		write(STDERR_FILENO, "\n", 1);
		exit(130);
	}
}

void	setup_heredoc_signals()
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = heredoc_sigint_handler;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}