#include "minishell.h"

// Builtin commands are executed in the parent process (not in a child).
// So, when we use dup2 to redirect input or output, the redirection affects the shell itself.
// To prevent this from breaking the shell, we save the original stdin/stdout first,
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

int	execute_builtin_cmd(char **argv, t_env **env) //should handle redir here!!!
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, env));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, env));
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(*env));
	// if (ft_strcmp(argv[0], "exit") == 0)
	// 	return (ft_exit(argv, *env));
	return (1);
}

void	run_builtin_with_redir(t_cmd *cmd, t_env **env_list)
{
	int	orig_stdin;
	int	orig_stdout;

	orig_stdin = dup(STDIN_FILENO); //save std to a tmp one
	orig_stdout = dup(STDOUT_FILENO);
	if (orig_stdin < 0 || orig_stdout < 0)
	{
		perror("dup failed");
		g_exit_status = 1;
		return ;
	}
	if (check_and_apply_redirections(cmd) < 0)
	{
		dup2(orig_stdin, STDIN_FILENO); //recover to std
		dup2(orig_stdout, STDOUT_FILENO);
		close(orig_stdin); //dup can generate a new fd, so i should close it after finishing
		close(orig_stdout);
		g_exit_status = 1;
		return ;
	}
	g_exit_status = execute_builtin_cmd(cmd->argv, env_list);
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	close(orig_stdin);
	close(orig_stdout);
}
