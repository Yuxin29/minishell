#include "minishell.h"

//called in build cmd list
int	check_get_one_new_cmd(t_exec_path *cmd, t_token *token_head, t_cmd *cmd_current, t_cmd *cmd_head)
{
	if (!token_head)
	{
		if (hd_is_interrupted())
		{
			free_cmd_node(cmd_current);
			free_cmd_list(cmd_head);
			cmd->exit_status = 130;
			return (1);
		}
		if (!cmd_current->argv && !cmd_current->redirections)
		{
			free_cmd_node(cmd_current);
			free_cmd_list(cmd_head);
			cmd->exit_status = 0;
			return (1);
		}
	}
	return (0);
}

t_cmd	*malloc_for_new_cmd(t_cmd *cmd_head)
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
	check_token_syntax(token_head, cmd);
	if (cmd->exit_status == 2)
		return (NULL);
	while (token_head)
	{
		cmd_current = malloc_for_new_cmd(cmd_head);
		if (!cmd_current)
			return (NULL);
		token_head = get_one_new_cmd(token_head, cmd_current);
		if (check_get_one_new_cmd(cmd, token_head, cmd_current, cmd_head))
			return (NULL);
		if (!cmd_head)
			cmd_head = cmd_current;
		else
			cmd_last->next = cmd_current;
		cmd_last = cmd_current;
	}
	return (cmd_head);
}

// used in get_one_new_cmd to skip the word tokens that have been parsed already
t_token	*loop_to_next(t_token *token_head)
{
	if (token_head && token_head->t_type == 0)
	{
		while (token_head && token_head->t_type == 0)
			token_head = token_head->next;
	}
	return (token_head);
}

// generate on single cmd struct
// parse_argv(cmd_current, argv_tok); 
// this function has a return value, need to chekc
t_token	*get_one_new_cmd(t_token *token_head, t_cmd *cmd_current)
{
	t_token	*argv_tok;

	while (token_head && token_head->t_type != 1)
	{
		if (token_head && token_head->t_type == 0)
		{
			argv_tok = token_head;
			parse_argv(cmd_current, argv_tok);
		}
		token_head = loop_to_next(token_head);
		if (!token_head)
			break ;
		if (token_head->t_type >= 2 && token_head->t_type <= 5)
		{
			token_head = parse_redirections(cmd_current, token_head);
			if (!token_head)
				return (NULL);
		}
		token_head = loop_to_next(token_head);
	}
	if (token_head && token_head->t_type == 1)
		token_head = token_head->next;
	return (token_head);
}
