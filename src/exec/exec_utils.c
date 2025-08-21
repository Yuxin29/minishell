#include "minishell.h"

static int	is_invalid_command(t_exec_path *exec_cmd, t_cmd *cmd,\
	t_env **env_list)
{
	if (cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", 2);
		free_all_and_exit(exec_cmd, env_list, 127);
		return (1);
	}
	if (ft_strncmp(cmd->argv[0], "..", 3) == 0)
	{
		ft_putendl_fd("..: command not found", 2);
		free_all_and_exit(exec_cmd, env_list, 127);
		return (1);
	}
	if (ft_strncmp(cmd->argv[0], ".", 2) == 0)
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		free_all_and_exit(exec_cmd, env_list, 2);
		return (1);
	}
	return (0);
}

void	handle_execve_or_exit_inchild(t_exec_path *exec_cmd, t_cmd *cmd,\
	t_env **env_list)
{
	if (is_invalid_command(exec_cmd, cmd, env_list))
		return ;
	if (!cmd->cmd_path)
	{
		if (ft_strchr(cmd->argv[0], '/'))
			precheck_path_or_exit(cmd->argv[0], exec_cmd, env_list);
		else
			print_error_and_exit(cmd, exec_cmd, env_list);
	}
	else
		precheck_path_or_exit(cmd->cmd_path, exec_cmd, env_list);
	execve(cmd->cmd_path, cmd->argv, exec_cmd->envp);
	perror("execve");
	free_t_exec_path(exec_cmd);
	free_env_list(env_list);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

void	free_all_and_exit(t_exec_path *cmd, t_env **env_list, int status)
{
	if (cmd)
		free_t_exec_path(cmd);
	if (env_list)
		free_env_list(env_list);
	exit (status);
}

void	free_all_and_exit_pipe(t_exec_path *cmd, t_env **env_list,\
	int status, t_pipe_ex *pinfo)
{
	free_t_exec_path(cmd);
	free_env_list(env_list);
	if (pinfo)
	{
		if (pinfo->prev_pipe != -1)
			close(pinfo->prev_pipe);
		close(pinfo->pipefd[0]);
		close(pinfo->pipefd[1]);
	}
	exit (status);
}

//for heredoc
char	*cleanup_heredoc(int fd, int saved_stdin, char *tmp_file, char *err_msg)
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
