#include "minishell.h"

int	g_exit_status = 0;

static int	check_invalid_cmds(t_cmd *cmd_list)
{
	t_cmd	*cur;

	cur = cmd_list;
	while (cur)
	{
		if (!cur->argv || !cur->argv[0] || cur->argv[0][0] == '\0')
		{
			if (cur->redirections)
			{
				if (check_and_apply_redirections(cur) == -1)
					g_exit_status = 1;
				else
					g_exit_status = 0;
				return (1);
			}
			else
			{
				ft_putstr_fd("minishell: : command not found\n", 2);
				g_exit_status = 127;
				return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_env		*env_list;
	char		*line;
	t_token		*token_list;
	t_exec_path	exec_cmd;
	t_cmd		*tmp;

	(void)argc;
	(void)argv;

    rl_catch_signals = 0;   // forbidden readline default signalsm global variables from the readline.h
	setup_signals();

	ft_memset(&exec_cmd, 0, sizeof(exec_cmd));
	env_list = env_list_init(envp);
	if (!env_list)
	{
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if(*line)
		{
			add_history(line);
			exec_cmd.envp = env_list_to_envp(env_list);
			if (!exec_cmd.envp)
			{
				free(line);
				free_env_list(env_list);
				ft_putstr_fd("Error: env list initialized failed\n", 2);
				exit(EXIT_FAILURE);
			}

			token_list = get_token_list(line); //covert line to token list
			free(line);
			if (!token_list)
			{
				ft_free_arr(exec_cmd.envp);
				if (g_exit_status == 2)
					continue;
				else
				{
					free_env_list(env_list);
					ft_putstr_fd("Error: get token list failed from memory failure\n", 2);
					exit(EXIT_FAILURE);
				}
			}

			exec_cmd.whole_cmd = build_command_list(token_list); //convert token list to command list
			free_token_list(token_list);
			if (!exec_cmd.whole_cmd)
			{
				ft_free_arr(exec_cmd.envp);
				if (g_exit_status == 2)
					continue;
				else
				{
					free_env_list(env_list);
					ft_putstr_fd("Error: build command list failed from memory failure\n", 2);
					exit(EXIT_FAILURE);
				}
			}

			expand_all_cmds(exec_cmd.whole_cmd, exec_cmd.envp);

			if (check_invalid_cmds(exec_cmd.whole_cmd))
			{
				free_t_exec_path(&exec_cmd);
				continue;
			}
			tmp = exec_cmd.whole_cmd;
			while (tmp)
			{
				if (!is_builtin(tmp->argv[0]))
					tmp->cmd_path = get_cmd_path(tmp->argv[0], env_list);
				else
					tmp->cmd_path = NULL;
				tmp = tmp->next;
			}

			//if bulitin and no pipe, no need to find cmd_path, just execute(need to deal with other things in it)
			if (is_builtin(exec_cmd.whole_cmd->argv[0]) && !exec_cmd.whole_cmd->next)
			{
				run_builtin_with_redir(exec_cmd.whole_cmd, &env_list);
				free_t_exec_path(&exec_cmd);
				continue;
			}
			if (exec_cmd.whole_cmd->next)
				execute_pipeline(&exec_cmd, env_list); //should check if builtin or external in it
			else
				execute_single_cmd(&exec_cmd);
			free_t_exec_path(&exec_cmd);
			continue;
		}
		rl_clear_history();
	}
	free_env_list(env_list);
	return (0);
}
