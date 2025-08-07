#include "minishell.h"

void	print_error_and_exit(t_cmd *cmd)
{
	if (!ft_strchr(cmd->argv[0], '/'))
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	else
	{
		perror(cmd->argv[0]);
		exit(127);
	}
}

static void	exec_single_child(t_exec_path *cmd)
{
	if (check_and_apply_redirections(cmd->whole_cmd) == -1)
			exit (EXIT_FAILURE); //once in fork, do not return, must exit
	if (!cmd->whole_cmd->cmd_path)
		print_error_and_exit(cmd->whole_cmd);
	execve(cmd->whole_cmd->cmd_path, cmd->whole_cmd->argv, cmd->envp);
	perror("execve");
	if (errno == EACCES) //should i use errno?
		exit(126); //permission denied， EACCEA means error:access
	else
		exit(127);
}

static void	wait_child_and_exit(t_exec_path *cmd, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) //if ture, meaning the process exit correctly
		cmd->exit_status = WEXITSTATUS(status); //get exit code
	else if (WIFSIGNALED(status)) //if child process be killed by signal
		cmd->exit_status = 128 + WTERMSIG(status); // killed by which signal, if killed by SIGINT(2),return 130 (128+2)
	else
		cmd->exit_status = 1; //show there's some problem
}

void	execute_single_cmd(t_exec_path *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_single_child(cmd);
	}
	else if (pid > 0)
		wait_child_and_exit(cmd, pid);
	else
	{
		perror("fork");
		cmd->exit_status = 1;
	}
}
