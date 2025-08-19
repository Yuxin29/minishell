#include "minishell.h"

// Builtin commands are executed in the parent process (not in a child).
// So, when we use dup2 to redirect input or output,
// the redirection affects the shell itself.
// To prevent this from breaking the shell,
// we save the original stdin/stdout first,
// and restore them after executing the builtin command.
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

//why close? dup can generate a new fd, so i should close it after finishing
static void	restore_stdio(int stdin_fd, int stdout_fd)
{
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
		perror("dup2 failed");
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
		perror("dup2 failed");
	close(stdin_fd);
	close(stdout_fd);
}

int	execute_builtin_cmd(char **argv, t_env **env, t_exec_path *exec_cmd)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, env));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(env));
	if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, env));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, env));
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(env));
	if (ft_strcmp(argv[0], "exit") == 0)
	{
		restore_stdio(exec_cmd->orig_std[0], exec_cmd->orig_std[1]);
		return (ft_exit(argv, exec_cmd, env));
	}
	return (1);
}

void	run_builtin_with_redir(t_exec_path *exec_cmd, t_env **env_list)
{
	exec_cmd->orig_std[0] = dup(STDIN_FILENO);
	exec_cmd->orig_std[1] = dup(STDOUT_FILENO);
	if (exec_cmd->orig_std[0] < 0 || exec_cmd->orig_std[1] < 0)
	{
		perror("dup failed");
		exec_cmd->exit_status = 1;
		return ;
	}
	if (check_and_apply_redirections(exec_cmd->whole_cmd) < 0)
	{
		restore_stdio(exec_cmd->orig_std[0], exec_cmd->orig_std[1]);
		exec_cmd->exit_status = 1;
		return ;
	}
	exec_cmd->exit_status = execute_builtin_cmd(
			exec_cmd->whole_cmd->argv, env_list, exec_cmd);
	restore_stdio(exec_cmd->orig_std[0], exec_cmd->orig_std[1]);
}
