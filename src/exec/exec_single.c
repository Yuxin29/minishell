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
	t_cmd *c = cmd->whole_cmd;
	struct stat st;

	if (check_and_apply_redirections(c) == -1)
		exit(EXIT_FAILURE);
	if (!c->argv || !c->argv[0])
		exit(0);
	if (!c->cmd_path)
		print_error_and_exit(c);
//
	{
		//struct stat st;
		if (stat(c->cmd_path, &st) == -1)
		{
			ft_putstr_fd(c->cmd_path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(c->cmd_path, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(c->cmd_path, X_OK) == -1)
		{
			ft_putstr_fd(c->cmd_path, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
//
	execve(c->cmd_path, c->argv, cmd->envp);
	if (errno == EACCES)
	{
		ft_putstr_fd(c->cmd_path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	exit(127);
}

// void	print_error_and_exit(t_cmd *cmd)
// {
// 	struct stat st;

// 	if (!ft_strchr(cmd->argv[0], '/'))
// 	{
// 		ft_putstr_fd(cmd->argv[0], 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		exit(127);
// 	}
// 	if (stat(cmd->argv[0], &st) == -1)
// 	{
// 		ft_putstr_fd(cmd->argv[0], 2);
// 		ft_putstr_fd(": No such file or directory\n", 2);
// 		exit(127);
// 	}
// 	if (S_ISDIR(st.st_mode))
// 	{
// 		ft_putstr_fd(cmd->argv[0], 2);
// 		ft_putstr_fd(": Is a directory\n", 2);
// 		exit(126);
// 	}
// 	if (access(cmd->argv[0], X_OK) == -1)
// 	{
// 		ft_putstr_fd(cmd->argv[0], 2);
// 		ft_putstr_fd(": Permission denied\n", 2);
// 		exit(126);
// 	}
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putstr_fd(cmd->argv[0], 2);
// 	ft_putstr_fd(": command not found\n", 2);
// 	exit(127);
// }


// static void	exec_single_child(t_exec_path *cmd)
// {
// 	if (check_and_apply_redirections(cmd->whole_cmd) == -1)
// 		exit (EXIT_FAILURE); //once in fork, do not return, must exit
// 	if (!cmd->whole_cmd->argv || !cmd->whole_cmd->argv[0])
// 		exit (0);
// 	if (!cmd->whole_cmd->cmd_path)
// 		print_error_and_exit(cmd->whole_cmd);
// 	execve(cmd->whole_cmd->cmd_path, cmd->whole_cmd->argv, cmd->envp);
// 	perror("execve");
// 	if (errno == EACCES || errno == EISDIR) //should i use errno?
// 		exit(126); //permission denied， EACCEA means error:access
// 	else
// 		exit(127);
// }

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
