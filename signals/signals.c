#include "../minishell.h"
#include "signals.h"

static void	set_handler(int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	sigaction(signum, &sa, NULL);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void	setup_signals(void)
{
	set_handler(SIGINT, handle_sigint, SA_RESTART);
	set_handler(SIGQUIT, SIG_IGN, 0);
	set_handler(SIGTTOU, SIG_IGN, 0);
	set_handler(SIGTTIN, SIG_IGN, 0);
}

void	setup_signals_child(void)
{
	set_handler(SIGINT, SIG_DFL, 0);
	set_handler(SIGQUIT, SIG_DFL, 0);

}
