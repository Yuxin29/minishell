#include "minishell.h"

#include <readline/readline.h>

// ✅ Expected minishell behavior
// Key	Expected behavior
// Ctrl-C	                Print newline, reset prompt, do not exit shell
// Ctrl-D	                If input is empty (i.e., user pressed Enter on empty line), exit shell
// Ctrl-\	                Should be ignored (like Bash)
// During fork() child	    Signals should behave like in real shell (e.g., Ctrl-C kills child)

// this one only called once in the main.
// hand coded in the exec child precess, eg when the cmd is sleep20
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint); //If my process receives a SIGINT (Ctrl-C), just ignore it (SIG_IGN = ignore)
	signal(SIGQUIT, SIG_IGN); //ignore SIGQUIT (usually triggered by Ctrl-\), so Ctrl-\ won’t kill my shell
}

//funciions in side this one is included in readline
//this one is used in signal(SIGINT, handle_sigint);
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);                 // get to next line
	rl_replace_line("", 0);            // empty this line
	rl_on_new_line();                  // prompt change to next line
	rl_redisplay();                    // display the new prompt
	//g_exit_status = 130;
}

