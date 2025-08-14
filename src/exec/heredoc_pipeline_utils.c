#include "minishell.h"

void	handle_execve_or_exit_inchild(t_exec_path *exec_cmd, t_cmd *cmd)
{
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (!cmd->cmd_path)
	{
		if (ft_strchr(cmd->argv[0], '/'))
			precheck_path_or_exit(cmd->argv[0]);
		else
			print_error_and_exit(cmd);//only deal with no /
	}
	else
		precheck_path_or_exit(cmd->cmd_path); //even if we got the path, still check the path
	execve(cmd->cmd_path, cmd->argv, exec_cmd->envp);
	perror("execve");
	if (errno == EACCES) //should i use errno?
		exit(126); //permission denied， EACCEA means error:access
	exit(127);
}

static void	wait_diff_status(int status, int *last_exit)
{
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		write(STDOUT_FILENO, "\n", 1);
		*last_exit = 128 + WTERMSIG(status);
	}
	else
		*last_exit = 1;
}

void	wait_exit(t_exec_path *exec_cmd, pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		last_exit;

	last_exit = 0;
	while (1) //While there is at least one child process still running, keep waiting for them to finish.if pass NULL, meaning “I don’t care about the exit status.
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == -1)
		{
			if (errno == EINTR) // Interrupted by signal, keep waiting
				continue ;
			if (errno == ECHILD) // No more child processes, done
				break ;
			exec_cmd->exit_status = 1; // Some unexpected error (e.g. internal waitpid failure)
			return ;
		}
		if (wpid == last_pid)
			wait_diff_status(status, &last_exit);
	}
	exec_cmd->exit_status = last_exit;
}

char	*cleanup_heredoc(int fd, int saved_stdin, char *tmp_file, const char *err_msg) //for heredoc
{
	if (err_msg)
		perror(err_msg);
	if (saved_stdin != -1)
		close(saved_stdin);
	if (fd != -1)
		close(fd);
	if (tmp_file)
	{
		unlink(tmp_file);
		free(tmp_file);
	}
	return (NULL);
}
