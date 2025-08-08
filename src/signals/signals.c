#include "minishell.h"
# include <signal.h>
# include <sys/types.h>
# include <unistd.h>

//volatile sig_atomic_t	g_signal = 0;
//signo (signal number): The integer code of the signal received. (For example, SIGINT == 2, SIGQUIT == 3), Every time the handler is called, signo tells you which signal was delivered.
// void	signal_handler(int signo)
// {
// 	g_signal = signo;
// 	write(1, "\n", 1);
// }

// // int	check_signal_and_reset(char **line)
// // {
// // 	if (g_signal == SIGINT)
// // 	{
// // 		rl_replace_line("", 0);// empty this line
// // 		printf("\n");// get to next line
// // 		rl_on_new_line();// prompt change to next line
// // 		rl_redisplay();// display the new prompt
// // 		g_signal = 0;
// // 		if (line && *line)
// // 			free(*line);
// // 		return (1);
// // 	}
// // 	return (0);
// // }

// // static int	rl_sig_hook(void)
// // {
// // 	if (g_signal == SIGINT)
// // 	{
// // 		rl_replace_line("", 0);
// // 		rl_on_new_line();
// // 		rl_redisplay();
// // 		g_signal = 0;
// // 	}
// // 	return (0);
// // }

// void	init_signals(void)
// {
// 	struct sigaction	sa;

// 	ft_memset(&sa, 0, sizeof(sa));
// 	sigemptyset(&sa.sa_mask);
// 	//"Most handlers do not need to block any signals (use sigemptyset), unless the handler is long or uses non-atomic resources."
// 	sa.sa_handler = signal_handler; // only sets g_signal
// 	sa.sa_flags = 0;
// 	sigaction(SIGINT, &sa, NULL); //how to deal with SIGINT(2)

// 	//ctrl+/
// 	sa.sa_handler = SIG_IGN;       // Ctrl-\ does nothing in interactive parent
// 	sigaction(SIGQUIT, &sa, NULL);

// 	rl_catch_signals = 0;
// }

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

void	signal_init(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

// void	signal_heredoc(void)
// {
// 	signal(SIGINT, handle_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// }

void	signal_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}


