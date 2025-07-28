#include "minishell.h"

//below 2 need to be move to utils later
void	error_and_return(const char *msg, t_exec_path *cmd_and_path)
{
    perror(msg);
	//need to free every thing here
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
	}
}

