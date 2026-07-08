#include "../minishell.h"
#include "signals.h"

static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
}

void	reset_signals(struct sigaction *old_int)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, old_int);
	sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void	restore_signals(struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
}
