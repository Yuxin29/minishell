#include "minishell.h"

void	signal_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	signal_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	hd_is_interrupted(void)
{
	if (g_signal)
	{
		g_signal = 0;
		return (1);
	}
	return (0);
}
