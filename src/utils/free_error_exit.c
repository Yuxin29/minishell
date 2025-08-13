#include "minishell.h"

//functions are are all called only before parsing and during parsing
// used in pre lexing/pre parsign checkings
void	errmsg_set_status(char *msg, t_exec_path *cmd)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	cmd->exit_status = 2;
}

//used in checking malloc strs or strdup, strjoin and so on
char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc: ");
	return (NULL);
}

void	free_cmd_node(t_cmd *c)
{
	if (!c)
		return ;
	if (c->argv)
		ft_free_arr(c->argv);
	if (c->quote_type)
		free(c->quote_type);
	if (c->redirections)
		free_redirections(c);
	if (c->cmd_path)
		free(c->cmd_path);
	free(c);
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
