#include "minishell.h"

int	g_exit_status = 0;

int main(int argc, char **argv, char **envp)
{
	t_env		*env_list;
	char		*line;
	t_token		*token_list;
	t_exec_path	exec_cmd;

	(void)argc;
	(void)argv;

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
			token_list = get_token_list(line); //covert line to token list
			//print_token_list(token_list);
			free(line);
			if (!token_list)
			{
				free_env_list(env_list);
				ft_putstr_fd("Error: get token list failed\n", 2);
				exit(EXIT_FAILURE);
			}
			exec_cmd.whole_cmd = build_command_list(token_list); //convert token list to command list
			free_token_list(token_list);
            //print_cmd_list(exec_cmd.whole_cmd);
			if (!exec_cmd.whole_cmd)
			{
				free_env_list(env_list);
				ft_putstr_fd("Error: build command list failed\n", 2);
                free_t_exec_path(&exec_cmd);
				exit(EXIT_FAILURE);
			}

			//check if argv is null, but redir is < infile
			if (!exec_cmd.whole_cmd->argv || !exec_cmd.whole_cmd->argv[0])
			{
				if (check_and_apply_redirections(exec_cmd.whole_cmd) == -1)
					g_exit_status = 1;
				else
					g_exit_status = 0;
				free_t_exec_path(&exec_cmd);
				continue;
			}
			//check if null or empty
			if (exec_cmd.whole_cmd->argv[0][0] == '\0')
			{
				ft_putstr_fd("minishell: : command not found\n", 2);
				g_exit_status = 127;
				free_t_exec_path(&exec_cmd);
				continue;
			}
			//get the copy of envp
			exec_cmd.envp = env_list_to_envp(env_list);
			if (!exec_cmd.envp)
			{
				free_env_list(env_list);
				free_t_exec_path(&exec_cmd);
				ft_putstr_fd("Error: env list initialized failed\n", 2);
				exit(EXIT_FAILURE);
			}
			//expand??
			//if bulitin and no pipe, no need to find cmd_path, just execute(need to deal with other things in it)
			if (is_builtin(exec_cmd.whole_cmd->argv[0]) && !exec_cmd.whole_cmd->next)
			{
				exec_cmd.cmd_path = NULL;
				run_builtin_with_redir(exec_cmd.whole_cmd, &env_list);
				free_t_exec_path(&exec_cmd);
				continue;
			}

			//if external
			//get cmd_path first, then run external cmd
			exec_cmd.cmd_path = get_cmd_path(exec_cmd.whole_cmd->argv[0], env_list);
			if (!exec_cmd.cmd_path)
			{
				if (!ft_strchr(exec_cmd.whole_cmd->argv[0], '/'))
				{
					ft_putstr_fd(exec_cmd.whole_cmd->argv[0], 2);
					ft_putstr_fd(": command not found\n", 2);
					g_exit_status = 127;
				}
				else
				{
					perror(exec_cmd.whole_cmd->argv[0]); //print no such file or dir
					g_exit_status = 127;
				}
				free_t_exec_path(&exec_cmd);
				continue;
			}
			if (exec_cmd.whole_cmd->next)
				execute_pipeline(&exec_cmd, env_list); //should check if builtin or external in it
			else
				execute_single_cmd(&exec_cmd, env_list);
			free_t_exec_path(&exec_cmd);
			continue;

		}
		//rl_clear_history();
	}
	free_env_list(env_list);
	return (0);
}
