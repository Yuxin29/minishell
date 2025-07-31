#include "minishell.h"

void	execute_external_cmd(t_exec_path *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (check_and_apply_redirections(cmd->whole_cmd) == -1)
			exit (EXIT_FAILURE); //once in fork, do not return, must exit
		execve(cmd->cmd_path, cmd->whole_cmd->argv, cmd->envp);
		perror("execve");
		//!!free everything before exit(or close??)
		 exit(127); //should i use errno?
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) //if ture, meaning the process exit correctly
			g_exit_status = WEXITSTATUS(status); //get exit code
		else if (WIFSIGNALED(status)) //if child process be killed by signal
			g_exit_status = 128 + WTERMSIG(status); // killed by which signal, if killed by SIGINT(2),return 130 (128+2)
		else
			g_exit_status = 1; //show there's some problem
	}
	else
	{
		perror("fork");
		g_exit_status = 1;
	}
}
