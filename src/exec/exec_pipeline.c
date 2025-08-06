#include "minishell.h"

static void	wait_exit(t_exec_path *exec_cmd, pid_t last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		exec_cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exec_cmd->exit_status = 128 + WTERMSIG(status);
	else
		exec_cmd->exit_status = 1;
	while (wait(NULL) > 0) //While there is at least one child process still running, keep waiting for them to finish.if pass NULL, meaning “I don’t care about the exit status.
		;
}

void	execute_pipeline(t_exec_path *exec_cmd, t_env *env_list)
{
	int		pipefd[2];
	pid_t	pid;
	int		prev_pipe;
	t_cmd	*cmd;
	pid_t	last_pid;

	prev_pipe = -1;
	cmd = exec_cmd->whole_cmd;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipefd) < 0)
			{
				perror("pipe failed");
				return ;//exit or assign g_exit_status and return??
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
		{
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (cmd->next)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			if (check_and_apply_redirections(cmd) == -1)
				exit(EXIT_FAILURE);
			if (is_builtin(cmd->argv[0]))
			{
				exec_cmd->exit_status = execute_builtin_cmd(cmd->argv, &env_list);
				exit(exec_cmd->exit_status);
			}
			else
			{
				if (!cmd->cmd_path)
					print_error_and_exit(cmd);
				execve(cmd->cmd_path, cmd->argv, exec_cmd->envp);
				perror("execve");
				if (errno == EACCES) //should i use errno?
					exit(126); //permission denied， EACCEA means error:access
				else
					exit(127);
			}
		}
		if (pid > 0)
		{
			if (prev_pipe != -1)
				close(prev_pipe);
			if (cmd->next)
			{
				prev_pipe = pipefd[0];
				close(pipefd[1]);
			}
			else
				prev_pipe = -1;
			last_pid = pid; //in the parent process, fork() returns the PID of the new child process, the pid hold the actual child's PID
			cmd = cmd->next;
		}
	}
	wait_exit(exec_cmd, last_pid); //wait_exit(last_pid); must be called outside the while loop, after all the pipeline commands have been forked.
}
