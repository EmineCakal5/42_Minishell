#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>

void	setup_signals(void);
void	setup_signals_child(void);
void	ignore_signals(void);

void	reset_signals(struct sigaction *old_int);
void	restore_signals(struct sigaction *old_int);

#endif
