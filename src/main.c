#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

//initiate env and enter cmd loop
int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	signal_init();
	env_list = env_list_init(envp);
	if (!env_list)
	{
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
	minishell_loop(&env_list);
	return (0);
}
