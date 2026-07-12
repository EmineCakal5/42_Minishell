#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>

/* signals/signals.c */
void	setup_signals(void);
void	setup_signals_child(void);
void	ignore_signals(void);

/* signals/signals_heredoc.c */
void	reset_signals(struct sigaction *old_int);
void	restore_signals(struct sigaction *old_int);

#endif
