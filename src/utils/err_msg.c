#include "minishell.h"

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

void	errmsg_set_status(char *msg, t_exec_path *cmd)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	cmd->exit_status = 2;
}

void	print_error(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
}
