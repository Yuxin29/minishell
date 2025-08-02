#include "minishell.h"

//I am not sure we need to exit here, maybe not
void	error_and_return(const char *msg, t_exec_path *cmd_and_path)
{
	perror(msg);
	free_t_exec_path(cmd_and_path);
	exit(EXIT_FAILURE);
}

void	free_t_exec_path(t_exec_path *cmd_and_path)
{
	if (cmd_and_path)
	{
		if (cmd_and_path->whole_cmd)
			free_cmd_list(cmd_and_path->whole_cmd);
		if (cmd_and_path->cmd_path)
			free(cmd_and_path->cmd_path);
		if (cmd_and_path->envp)
			ft_free_arr(cmd_and_path->envp);
		ft_memset(cmd_and_path, 0, sizeof(*cmd_and_path));
	}
}
