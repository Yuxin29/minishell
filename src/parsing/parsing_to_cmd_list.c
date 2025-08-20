#include "minishell.h"

//called in build cmd list
static int	check_new_cmd(t_exec_path *cmd,\
	t_token *token, t_cmd *cmd1, t_cmd *cmd2)
{
	if (!token)
	{
		if (hd_is_interrupted())
		{
			free_cmd_node(cmd1);
			free_cmd_list(cmd2);
			cmd->exit_status = 130;
			return (1);
		}
		if (!cmd1->argv && !cmd1->redirections)
		{
			free_cmd_node(cmd1);
			free_cmd_list(cmd2);
			cmd->exit_status = 0;
			return (1);
		}
	}
	return (0);
}

// called in build_command_list
static t_cmd	*malloc_for_new_cmd(t_cmd *cmd_head)
{
	t_cmd	*cmd_current;

	cmd_current = malloc(sizeof(t_cmd));
	if (!cmd_current)
	{
		free_cmd_list(cmd_head);
		return ((t_cmd *)free_malloc_fail_null(NULL));
	}
	ft_bzero(cmd_current, sizeof(t_cmd));
	return (cmd_current);
}

//to find / recognize a cmd, we need to find a pipe
//basically, before the pipe is a cmd, after it is pipe.
//if no pipe, then this is a single cmd
//no need to null check token head, if null, exited earlier
t_cmd	*build_command_list(t_exec_path *cmd, t_token *token_head)
{
	t_cmd	*cmd_head;
	t_cmd	*cmd_current;
	t_cmd	*cmd_last;

	cmd_head = NULL;
	cmd_last = NULL;
	while (token_head)
	{
		cmd_current = malloc_for_new_cmd(cmd_head);
		if (!cmd_current)
			return (NULL);
		token_head = get_one_new_cmd(token_head, cmd_current, cmd);
		if (check_new_cmd(cmd, token_head, cmd_current, cmd_head))
			return (NULL);
		if (!cmd_head)
			cmd_head = cmd_current;
		else
			cmd_last->next = cmd_current;
		cmd_last = cmd_current;
	}
	return (cmd_head);
}
