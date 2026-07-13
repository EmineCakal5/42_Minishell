/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:59 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:29:00 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "signals.h"

static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
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
