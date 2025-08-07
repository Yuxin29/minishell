#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>
extern volatile sig_atomic_t g_sig_num;

//sig_atomic_t: An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts.

int		check_signanl_and_reset(char *line);
void	init_signals(void)


#endif

// we dont need to define this one: from signal.g
// struct sigaction
// {
// 	void     (*sa_handler)(int);
// 	sigset_t  sa_mask;
// 	int       sa_flags;
// 	// possibly sa_restorer, or sa_sigaction for advanced use
// };
