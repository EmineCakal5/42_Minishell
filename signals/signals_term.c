#include "../minishell.h"
#include "signals.h"

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
