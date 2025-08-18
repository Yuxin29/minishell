#include "minishell.h"

// syntaxprecheck before build cmd list
// can not start or end with a pipe
// after redirections it can not be followed by pipe or redirectiosn
// after pipe, it can not be followed by another pipe
// at the end of the tokens, it can not be pipe or redirections
// if (!token_head)
	// 	return (1); this already check in main??
void	check_token_syntax(t_token *token_head, t_exec_path *cmd)
{
	cmd->exit_status = 0;
	if (token_head->t_type == 1)
		return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd));
	while (token_head)
	{
		if (token_head->t_type >= 1 && token_head->next
			&& token_head->next->t_type == 1)
			return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd));
		if (token_head->t_type >= 2 && token_head->next
			&& token_head->next->t_type >= 2)
			return (errmsg_set_status(SYNTAX_ERR_REDIR_DOUBLE, cmd));
		if (token_head->t_type == 1 && token_head->next == NULL)
			return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd));
		if (token_head->t_type >= 2 && token_head->next == NULL)
			return (errmsg_set_status(SYNTAX_ERR_REDIR_FILE_MISSING, cmd));
		token_head = token_head->next;
	}
}

//used in parse_argv, for malloc str of strs
int	count_argv(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->t_type != 1)
	{
		if (start->t_type == 0)
			count++;
		else if (start->t_type >= 2 && start->t_type <= 5)
			start = start->next;
		start = start->next;
	}
	return (count);
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

t_token	*parse_if_no_argv(t_cmd *cmd_current, t_token *token_head)
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
