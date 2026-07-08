#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>

/*
** signals/signals.c            (YAZILACAK)
**
**   setup_signals()
**     — Ana dongu icin:
**         SIGINT  -> prompt'u temizle, yeni satir yaz
**         SIGQUIT -> yoksay (SIG_IGN)
**
**   setup_signals_child()
**     — fork() sonrasi child surec icin:
**         SIGINT  -> varsayilan (SIG_DFL)
**         SIGQUIT -> varsayilan (SIG_DFL)
**
**   reset_signals()
**     — Heredoc okuma sirasinda ozel davranis.
setup_signals(): Shell ana döngüsü için (Ctrl+C => Yeni satır).

setup_signals_child(): Komutlar çalışırken (Ctrl+C => Komutu öldür).

reset_signals(): Heredoc okuma moduna geçerken (Özel davranış).
*/
/* signals/signals.c */
void	setup_signals(void);
void	setup_signals_child(void);

/* signals/signals_heredoc.c */
void	reset_signals(struct sigaction *old_int);
void	restore_signals(struct sigaction *old_int);

/* signals/signals_term.c */
pid_t	get_terminal_pgrp(void);
void	set_terminal_pgrp(pid_t pgrp);

#endif
