#include "minishell.h"

volatile sig_atomic_t g_signal_status = 0;

void interactive_sigint_handler(int sig)
{
	g_signal_status = sig;
	if (sig == SIGINT)
	{
		// write(STDOUT_FILENO, "\n", 1);
		rl_done = 1;
		rl_replace_line("", 0);
		// rl_on_new_line();
		// rl_redisplay();
	}
}

void setup_signal_handlers(void)
{
	rl_catch_signals = 0; // Disable readline's default signal handling
	
	struct sigaction sa_int;
	sa_int.sa_handler = interactive_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0; // Restart interrupted system calls
	sigaction(SIGINT, &sa_int, NULL);   // Ctrl-C
	signal(SIGQUIT, SIG_IGN); // Ctrl-\ ignore SIGQUIT signal
}

// Signal handler during command execution
void execution_sigint_handler(int sig)
{
    (void)sig;
    // During execution, we want default behavior for child processes
    // This handler should do minimal work
    g_signal_status = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void setup_execution_signals(void)
{
    struct sigaction sa_int, sa_quit;
    
    // During execution, ignore SIGINT and SIGQUIT in parent
    // Child processes will reset to default in execute_child
    // sa_int.sa_handler = execution_sigint_handler;
    sa_int.sa_handler = SIG_IGN;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
