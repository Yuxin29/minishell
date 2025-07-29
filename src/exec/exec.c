#include "minishell.h"
#include "parsing.h"

int	execute_internal_cmd(t_exec_path *cmd)
{
	pid_t	pid;
	int		status;

	// if (is_builtin(cmd->whole_cmd->argv[0]))
	// {
	// 	//check_and_apply_redirections(cmd->whole_cmd); //added by yuxin
	// 	return (execute_builtin(cmd->whole_cmd->argv, env)); //modify
	// }
	if (!cmd || !cmd->whole_cmd || !cmd->whole_cmd->argv || !cmd->whole_cmd->argv[0]) //yuxing added
		return (0);
	pid = fork();
	if (pid == 0)
	{
		check_and_apply_redirections(cmd->whole_cmd); //added by yuxin
		//check_and_apply_heredocs(cmd->whole_cmd);
		execve(cmd->cmd_path, cmd->whole_cmd->argv, cmd->envp);
		//free close??
		perror("execve");
		exit(128); // 127or128??
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
			return (WEXITSTATUS(status));
	}
	else
	{
		//free, close??
		perror("fork");
		return (-1);
	}
}
