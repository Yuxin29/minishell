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
