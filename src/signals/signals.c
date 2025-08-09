#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

//volatile sig_atomic_t	g_signal = 0;

//signo (signal number): The integer code of the signal received. (For example, SIGINT == 2, SIGQUIT == 3), Every time the handler is called, signo tells you which signal was delivered.

//signal(SIGINT, SIG_IGN); //If my process receives a SIGINT (Ctrl-C), just ignore it (SIG_IGN = ignore)
//signal(SIGQUIT, SIG_IGN); //ignore SIGQUIT (usually triggered by Ctrl-\), so Ctrl-\ won’t kill my shell


static void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
static void	handle_heredoc(int sig)
{
	(void)sig;
	g_signal = 1;
	//write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	signal_init(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_heredoc(void)
{
	signal(SIGINT, handle_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
