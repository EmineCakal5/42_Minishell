/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:29:04 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:29:05 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
