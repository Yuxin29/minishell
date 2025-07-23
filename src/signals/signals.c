#include "minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, SIG_IGN); //If my process receives a SIGINT (Ctrl-C), just ignore it (SIG_IGN = ignore)
	signal(SIGQUIT, SIG_IGN); //ignore SIGQUIT (usually triggered by Ctrl-\), so Ctrl-\ won’t kill my shell
}

/*void (*signal(int sig, void (*func)(int)))(int);
It takes two parameters:

    The signal number (sig)

    The function to handle that signal (func)

It returns the previous handler for that signal.*/
