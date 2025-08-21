#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

//initiate env and enter cmd loop
static void	minishell_loop(t_env **env_list)
{
	t_exec_path	exec_cmd;
	char		*line;

	ft_memset(&exec_cmd, 0, sizeof(exec_cmd));
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			handle_line(line, env_list, &exec_cmd);
		}
	}
	rl_clear_history();
	free_env_list(env_list);
}

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
