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

static void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void setup_signals(void)
{
	set_handler(SIGINT, handle_sigint, SA_RESTART);
	set_handler(SIGQUIT, SIG_IGN, 0);
	set_handler(SIGTTOU, SIG_IGN, 0);
	set_handler(SIGTTIN, SIG_IGN, 0);
}

void setup_signals_child(void)
{
	set_handler(SIGINT, SIG_DFL, 0);
	set_handler(SIGQUIT, SIG_DFL, 0);

}

static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
}

void reset_signals(struct sigaction *old_int)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, old_int);
	sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void restore_signals(struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
}

pid_t	get_terminal_pgrp(void)
{
	if (!isatty(STDIN_FILENO))
		return (-1);
	return (tcgetpgrp(STDIN_FILENO));
}

void	set_terminal_pgrp(pid_t pgrp)
{
	if (pgrp < 0 || !isatty(STDIN_FILENO))
		return ;
	tcsetpgrp(STDIN_FILENO, pgrp);
}
