#include "minishell.h"

static int	create_pipe_or_exit(t_exec_path *exec_cmd, t_pipe_ex *pinfo)
{
	if (pipe(pinfo->pipefd) < 0)
	{
		perror("pipe failed");
		exec_cmd->exit_status = 1;
		return (0);
	}
	return (1);
}

static void	handle_child_process(t_exec_path *exec_cmd,
	t_cmd *cmd, t_env *env_list, t_pipe_ex *pinfo)
{
	int	i;

	signal_default();
	if (pinfo->prev_pipe != -1)
	{
		if (dup2(pinfo->prev_pipe, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(EXIT_FAILURE);
		}
		close(pinfo->prev_pipe);
	}
	if (cmd->next)
	{
		if (dup2(pinfo->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(EXIT_FAILURE);
		}
		close(pinfo->pipefd[0]);
		close(pinfo->pipefd[1]);
	}
	if (check_and_apply_redirections(cmd) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->argv[0]))
	{
		i = execute_builtin_cmd(cmd->argv, &env_list, exec_cmd);
		free_two(exec_cmd, &env_list);
		exit(i);
	}
	handle_execve_or_exit_inchild(exec_cmd, cmd, env_list);
}

//in the parent process, fork() returns the PID of the new child process,
//the pid hold the actual child's PID, so pinfo->last_pid = pid;
static void	handle_parent_process(t_cmd *cmd, t_pipe_ex *pinfo, pid_t pid)
{
	if (pinfo->prev_pipe != -1)
		close(pinfo->prev_pipe);
	if (cmd->next)
	{
		pinfo->prev_pipe = pinfo->pipefd[0];
		close(pinfo->pipefd[1]);
	}
	else
		pinfo->prev_pipe = -1;
	pinfo->last_pid = pid;
}

static int	fork_and_exec(t_exec_path *exec_cmd, t_cmd *cmd,
	t_pipe_ex *pinfo, t_env *env_list)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		if (cmd->next)
		{
			close(pinfo->pipefd[0]);
			close(pinfo->pipefd[1]);
		}
		if (pinfo->prev_pipe != -1)
			close(pinfo->prev_pipe);
		exec_cmd->exit_status = 1;
		return (0);
	}
	if (pid == 0)
		handle_child_process(exec_cmd, cmd, env_list, pinfo);
	if (pid > 0)
		handle_parent_process(cmd, pinfo, pid);
	return (1);
}

//wait_exit(last_pid); must be called outside the while loop,
//after all the pipeline commands have been forked.
void	execute_pipeline(t_exec_path *exec_cmd, t_env *env_list)
{
	t_cmd		*cmd;
	t_pipe_ex	pinfo;

	pinfo.prev_pipe = -1;
	cmd = exec_cmd->whole_cmd;
	while (cmd)
	{
		if (cmd->next && !create_pipe_or_exit(exec_cmd, &pinfo))
			return ;
		if (!fork_and_exec(exec_cmd, cmd, &pinfo, env_list))
			return ;
		cmd = cmd->next;
	}
	signal_ignore();
	wait_exit(exec_cmd, pinfo.last_pid);
	signal_init();
}
