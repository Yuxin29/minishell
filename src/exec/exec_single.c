#include "minishell.h"

void	print_error_and_exit(t_cmd *cmd)
{
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	precheck_path_or_exit(char *path)
{
	struct stat	st; //a structure that holds detailed information about a file.（man 2 stat)
	if (stat(path, &st) == -1)
	{
		ft_putstr_fd(path, 2); //It checks the file at path and fills the struct stat (st) with its info.
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	if (S_ISDIR(st.st_mode)) //st_mode:what kind of file it is; what perssion it has;  S_ISDIR:Is this file a directory?
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if (access(path, X_OK) == -1) // it checks whether the file can be accessed with the given mode:
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
}

static void	exec_single_child(t_exec_path *cmd)
{
	t_cmd	*c;

	c = cmd->whole_cmd;
	if (check_and_apply_redirections(c) == -1)
		exit(EXIT_FAILURE);
	if (!c->argv || !c->argv[0]) //if redir ok but no argv, still 0
		exit(0);
	if (c->argv[0][0] == '\0') //""
	{
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (!c->cmd_path)
	{
		if (ft_strchr(c->argv[0], '/'))
			precheck_path_or_exit(c->argv[0]);
		else
			print_error_and_exit(c);//only deal with no /
	}
	else
		precheck_path_or_exit(c->cmd_path); //even if we got the path, still check the path
	execve(c->cmd_path, c->argv, cmd->envp);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

static void	wait_child_and_exit(t_exec_path *cmd, pid_t pid)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1 && errno == EINTR) //Interrupted system call
		continue ;//even id the waitpid be interupted by ctrl c, the waitpid still need to goning on
	if (WIFEXITED(status)) //if ture, meaning the process exit correctly
		cmd->exit_status = WEXITSTATUS(status); //get exit code
	else if (WIFSIGNALED(status)) ///if child process be killed by signal
	{
		write(STDOUT_FILENO, "\n", 1);
		cmd->exit_status = 128 + WTERMSIG(status); //// killed by which signal, if killed by SIGINT(2),return 130 (128+2)
	}
	else
		cmd->exit_status = 1; //show there's some problem
}

void	execute_single_cmd(t_exec_path *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal_default();
		exec_single_child(cmd);
	}
	else if (pid > 0)
	{
		signal_ignore();
		wait_child_and_exit(cmd, pid);
		signal_init();
	}
	else
	{
		perror("fork");
		cmd->exit_status = 1;
	}
}
