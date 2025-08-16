#include "minishell.h"

// use for syntax cheack in before lexing and parsing
void	errmsg_set_status(char *msg, t_exec_path *cmd)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	cmd->exit_status = 2;
}

//for cd
int	perror_return_one(char *str)
{
	if (str)
		perror(str);
	return (1);
}

//for cd
int	errmsg_return_one(char *str)
{
	if (str)
		ft_putendl_fd(str, 2);
	return (1);
}

// used for export error msg
void	print_error(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
}

//used in main
int	handle_token_build_failure(t_exec_path *exec_cmd, t_env **env_list)
{
	ft_free_arr(exec_cmd->envp);
	if (exec_cmd->exit_status == 2 || exec_cmd->exit_status == 0)
		return (0);
	free_env_list(*env_list);
	ft_putstr_fd("Error: get token list failed from memory failure\n", 2);
	exit(EXIT_FAILURE);
}
