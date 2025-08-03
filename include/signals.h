#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>

/*
we dont need to define this one: from signal.g
struct sigaction 
{
	void     (*sa_handler)(int);
	sigset_t  sa_mask;
	int       sa_flags;
	// possibly sa_restorer, or sa_sigaction for advanced use
};*/

void	setup_signals(void);
void	handle_sigint(int sig);

#endif
