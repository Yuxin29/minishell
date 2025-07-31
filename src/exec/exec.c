#include "minishell.h"

int	execute_external_cmd(t_exec_path *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (check_and_apply_redirections(cmd->whole_cmd) == -1) //redirect should happen after fork and before execute
			exit (EXIT_FAILURE); //once in fork, do not return, must exit
		execve(cmd->cmd_path, cmd->whole_cmd->argv, cmd->envp);
		perror("execve");
		//!!free everything before exit(or close??)
		 exit(127);
		// if (errno == ENOENT)
		// 	exit(127);
		// else
		// 	exit(126);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
			return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		return (-1);
	}
}
