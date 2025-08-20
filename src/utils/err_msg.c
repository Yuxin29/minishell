#include "minishell.h"

// use for syntax cheack in before lexing and parsing
// set status to 2 for syntax error
void	errmsg_set_status(char *msg, t_exec_path *cmd)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	cmd->exit_status = 2;
}

// 1 for perror, 0 for stderr
int	errmsg_return_nbr(char *str, int i, int nbr)
{
	if (i)
	{
		if (str)
			perror(str);
	}
	else
	{
		if (str)
			ft_putendl_fd(str, 2);
	}
	return (nbr);
}

// used for export error msg
void	print_error(char *str1, const char *arg, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(str3, 2);
}

// used in main
int	handle_token_build_failure(t_exec_path *exec_cmd, t_env **env_list)
{
	ft_free_arr(exec_cmd->envp);
	if (exec_cmd->exit_status == 2 || exec_cmd->exit_status == 0)
		return (0);
	free_env_list(*env_list);
	ft_putstr_fd("Error: get token list failed from memory failure\n", 2);
	exit(EXIT_FAILURE);
}

// return the length of a valid variable name
// not malloc involved
// $ skipped already
int	var_name_len(const char *str)
{
	int	len;

	len = 0;
	if (!str[len])
		return (len);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (len);
	while ((str[len]) && str[len] != '=')
		len++;
	return (len);
}

