#include "minishell.h"

#include "signal.h"

#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;
/*
SIGINT:2(Ctrl-C)
SIGQUIT:3(Ctrl-\)
SIGTERM:15
SIGKILL:9
SIGUSR1、SIGUSR2: defined
*/
//signo (signal number): The integer code of the signal received. (For example, SIGINT == 2, SIGQUIT == 3), Every time the handler is called, signo tells you which signal was delivered.
void	signal_handler(	int signo)
{
	g_signal = signo;
}

int	check_signanl_and_reset(char *line)
{
	if (g_signal == SIGINT)
	{
		write(1, "\n", 1);                 // get to next line
		rl_replace_line("", 0);         // empty this line
		rl_on_new_line();                  // prompt change to next line
		rl_redisplay();                    // display the new prompt
		g_signal = 0;
		if (line)
			free(line);
		return (1);
	}
	return (0);
}

void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	//ctrl+c
	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	//"Most handlers do not need to block any signals (use sigemptyset), unless the handler is long or uses non-atomic resources."
	sa_int.sa_flags = SA_RESTART;
	// #define SA_RESTART      restart system on signal return, the readline can be restarted
	sigaction(SIGINT, &sa_int, NULL); //how to deal with SIGINT(2)

	//ctrl+/
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL); //how to deal with SIGQUIT(3)
}

/*struct sigaction {
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};*/

//signal(SIGINT, SIG_IGN); //If my process receives a SIGINT (Ctrl-C), just ignore it (SIG_IGN = ignore)
//signal(SIGQUIT, SIG_IGN); //ignore SIGQUIT (usually triggered by Ctrl-\), so Ctrl-\ won’t kill my shell
