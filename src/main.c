#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	run_command(t_exec_path *exec_cmd, t_env **env_list)
{
	t_cmd	*tmp;

	tmp = exec_cmd->whole_cmd;
	while (tmp)
	{
		if (!tmp->argv || !tmp->argv[0])
			tmp->cmd_path = NULL;
		else if (!is_builtin(tmp->argv[0]))
			tmp->cmd_path = get_cmd_path(tmp->argv[0], *env_list);
		else
			tmp->cmd_path = NULL;
		tmp = tmp->next;
	}
	if (!exec_cmd->whole_cmd->next)
	{
		if (!exec_cmd->whole_cmd->argv || !exec_cmd->whole_cmd->argv[0])
			execute_single_cmd(exec_cmd);
		else if (is_builtin(exec_cmd->whole_cmd->argv[0]))
			run_builtin_with_redir(exec_cmd, env_list);
		else
			execute_single_cmd(exec_cmd);
	}
	else
		execute_pipeline(exec_cmd, *env_list);
}

static int	parse_and_expand(t_exec_path *exec_cmd, char *expanded_line, t_env **env_list)
{
	t_token *token_list;

	token_list = get_token_list(exec_cmd, expanded_line); //covert line to token list
	free(expanded_line);
	if (!token_list)
	{
		if (!handle_token_build_failure(exec_cmd, env_list))
			return (0);
	}
	exec_cmd->whole_cmd = build_command_list(exec_cmd, token_list); //convert token list to command list
	free_token_list(token_list);
	if (!exec_cmd->whole_cmd)
	{
		ft_free_arr(exec_cmd->envp);
		if (exec_cmd->exit_status == 2 || exec_cmd->exit_status == 130)
			return (0);
		free_env_list(*env_list);
		ft_putstr_fd("Error: build command list failed from memory failure\n", 2);
		exit(EXIT_FAILURE);
	}
	expand_all_cmds(exec_cmd, exec_cmd->whole_cmd, exec_cmd->envp);
	return (1);
}
static void	handle_line(char *line, t_env **env_list, t_exec_path *exec_cmd)
{
	char	*expanded_line;

	exec_cmd->envp = env_list_to_envp(*env_list);
	if (!exec_cmd->envp)
	{
		free(line);
		free_env_list(*env_list);
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
	expanded_line = pre_expand_line(exec_cmd, line); //null check this one
	free(line);
	if (!expanded_line)
	{
		ft_free_arr(exec_cmd->envp);
		free_env_list(*env_list);
		ft_putstr_fd("Error: pre expanding failure\n", 2);
		exit(EXIT_FAILURE);
	}
	if (!parse_and_expand(exec_cmd, expanded_line, env_list))
		return ;
	run_command(exec_cmd, env_list);
	free_t_exec_path(exec_cmd);
}

static void	minishell_loop(t_env *env_list)
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
		if(*line)
		{
			add_history(line);
			handle_line(line, &env_list, &exec_cmd);
		}
	}
	rl_clear_history();
	free_env_list(env_list);
}

int main(int argc, char **argv, char **envp)
{
	t_env		*env_list;

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
	minishell_loop(env_list);
	return (0);
}
