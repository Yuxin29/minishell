#include "minishell.h"

void	print_error_and_exit(t_cmd *cmd, t_exec_path *exec_cmd, t_env **env_list)
{
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (free_all_and_exit(exec_cmd, env_list, 127));
}

//a structure that holds detailed information about a file.（man 2 stat)
//stat checks the file at path and fills the struct stat (st) with its info.
//st_mode:what kind of file it is; what perssion it has;
// S_ISDIR:Is this file a directory
//access checks whether the file can be accessed with the given mode
void	precheck_path_or_exit(char *path, t_exec_path *cmd, t_env **env_list)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (free_all_and_exit(cmd, env_list, 127));
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (free_all_and_exit(cmd, env_list, 126));
	}
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (free_all_and_exit(cmd, env_list, 126));
	}
}

//line 44: if redir ok but no argv, still 0
//line 60:even if we got the path, still check the path
static void	exec_single_child(t_exec_path *cmd, t_env **env_list)
{
	t_cmd	*c;

	c = cmd->whole_cmd;
	if (check_and_apply_redirections(c) == -1)
		return (free_all_and_exit(cmd, env_list, 1));
	if (!c->argv || !c->argv[0])
		return (free_all_and_exit(cmd, env_list, 0));
	if (c->argv[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", 2);
		return (free_all_and_exit(cmd, env_list, 127));
	}
	if (!c->cmd_path) //should free inside function
	{
		if (ft_strchr(c->argv[0], '/'))
			precheck_path_or_exit(c->argv[0], cmd, env_list);
		else
			print_error_and_exit(c, cmd, env_list);
	}
	else
		precheck_path_or_exit(c->cmd_path, cmd, env_list);
	execve(c->cmd_path, c->argv, cmd->envp);
	free_two(cmd, env_list);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

//eintr: Interrupted
//even id the waitpid be interupted by ctrl c,
// the waitpid still need to goning on
//line 80:killed by which signal, if killed by SIGINT(2),return 130 (128+2)
static void	wait_child_and_exit(t_exec_path *cmd, pid_t pid)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		continue ;
	if (WIFEXITED(status))
		cmd->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		write(STDOUT_FILENO, "\n", 1);
		cmd->exit_status = 128 + WTERMSIG(status);
	}
	else
		cmd->exit_status = 1;
}

void	execute_single_cmd(t_exec_path *cmd, t_env **env_list)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal_default();
		exec_single_child(cmd, env_list);
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
		signal_init();
		return ;
	}
}
