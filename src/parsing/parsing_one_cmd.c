#include "minishell.h"

// used in get_one_new_cmd to skip the word tokens that have been parsed already
static t_token	*loop_to_next(t_token *token_head)
{
	if (token_head && token_head->t_type == 0)
	{
		while (token_head && token_head->t_type == 0)
			token_head = token_head->next;
	}
	return (token_head);
}

static t_token	*parse_if_no_argv(t_cmd *cmd_current, t_token *token_head)
{
	if (token_head && token_head->t_type == 0 && !cmd_current->argv)
		parse_argv(cmd_current, token_head);
	return (token_head);
}

// generate on single cmd struct
// parse_argv(cmd_current, argv_tok);
// this function has a return value, need to chekc
t_token	*get_one_new_cmd(t_token *token_head, t_cmd *cmd_current,
	t_exec_path *exec_cmd)
{
	t_token	*argv_tok1;

	while (token_head && token_head->t_type != 1)
	{
		argv_tok1 = token_head;
		token_head = parse_if_no_argv(cmd_current, argv_tok1);
		token_head = loop_to_next(token_head);
		if (!token_head)
			break ;
		if (token_head->t_type >= 2 && token_head->t_type <= 5)
		{
			token_head = parse_redirections(cmd_current, token_head, exec_cmd);
			if (!token_head)
				return (NULL);
		}
		if (token_head && token_head->t_type == 0 && !cmd_current->argv)
			token_head = parse_if_no_argv(cmd_current, token_head);
		else
			token_head = loop_to_next(token_head);
	}
	if (token_head && token_head->t_type == 1)
		token_head = token_head->next;
	return (token_head);
}
