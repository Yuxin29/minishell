#include "minishell.h"

int	create_pipe_or_exit(t_exec_path *exec_cmd, t_pipe_ex *pinfo)
{
	if (pipe(pinfo->pipefd) < 0)
	{
		perror("pipe failed");
		exec_cmd->exit_status = 1;
		return (0);
	}
	return (1);
}

int	set_up_stdin(t_pipe_ex *pinfo, t_exec_path *cmd, t_env **env_list)
{
	if (pinfo->prev_pipe == -1)
		return (1);
	if (dup2(pinfo->prev_pipe, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		free_all_and_exit_pipe(cmd, env_list, 1, pinfo);
	}
	close (pinfo->prev_pipe);
	return (1);
}

int	set_up_stdout(t_cmd *cmd, t_pipe_ex *pinfo, t_exec_path *exec_cmd,\
	t_env **env_list)
{
	if (!cmd->next)
		return (1);
	if (dup2(pinfo->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 stdin");
		free_all_and_exit_pipe(exec_cmd, env_list, 1, pinfo);
	}
	close(pinfo->pipefd[1]);
	close(pinfo->pipefd[0]);
	return (1);
}

static void	wait_diff_status(int status, int *last_exit)
{
	int	sig;

	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		if (sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		*last_exit = 128 + sig;
	}
	else
		*last_exit = 1;
}

//while 1: //While there is at least one child process still running,
// keep waiting for them to finish.if pass NULL,
// meaning “I don’t care about the exit status.
//line 56: Interrupted by signal, keep waiting
//line 59: No more child processes, done
//62: Some unexpected error (e.g. internal waitpid failure)
void	wait_exit(t_exec_path *exec_cmd, pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		last_exit;

	last_exit = 0;
	while (1)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno == ECHILD)
				break ;
			exec_cmd->exit_status = 1;
			return ;
		}
		if (wpid == last_pid)
			wait_diff_status(status, &last_exit);
	}
	exec_cmd->exit_status = last_exit;
}
