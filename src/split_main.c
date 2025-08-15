#include "minishell.h"

static void	run_cmd_list(t_exec_path *exec_cmd, t_env **env_list)
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
		execute_pipeline(&exec_cmd, *env_list);
}
