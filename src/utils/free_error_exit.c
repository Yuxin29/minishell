#include "minishell.h"

//functions are are all called only before parsing and during parsing
// used in pre lexing/pre parsign checkings
int	errmsg_return_one(char *msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	return (1);
}

//used in checking malloc strs or strdup, strjoin and so on
char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc: ");
	return (NULL);
}

//lin used in main
void	free_t_exec_path(t_exec_path *cmd_and_path)
{
	if (cmd_and_path)
	{
		if (cmd_and_path->whole_cmd)
			free_cmd_list(cmd_and_path->whole_cmd);
		if (cmd_and_path->envp)
			ft_free_arr(cmd_and_path->envp);
	}
}
