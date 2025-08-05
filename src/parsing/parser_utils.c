
#include "parsing.h"
#include "minishell.h"

extern int g_exit_status;

// if it is a word without quotes, it can not contain special character
int	check_special_characters(t_token *token_head)
{
	if (token_head->t_type == 0 && token_head->quote_type == 0)
	{
		if (ft_strchr((const char *)token_head->str, ';')
			|| ft_strchr((const char *)token_head->str, '\\'))
			return (1);
	}
	return (0);
}

//precheck before build cmd list
// syntax check:
// can not start or end with a pipe
// after redirections it can not be followed by pipe or redirectiosn
// after pipe, it can not be followed by pipe or redirections
// at the end of the tokens, it can not be pipe or redirections
int	check_token_syntax(t_token *token_head)
{
	if (!token_head)
		return (1);
	if (token_head->t_type == 1)
		errmsg_set_status("minishell: syntax error near unexpected token `|'");
	while (token_head)
	{
		if (check_special_characters(token_head))
			errmsg_set_status("minishell: syntax error: special characters");
		if (token_head->t_type >= 1 && token_head->next && token_head->next->t_type == 1)
			errmsg_set_status("minishell: ssyntax error near unexpected token `|'");
		if (token_head->t_type == 1 && token_head->next && token_head->next->t_type > 1)
			errmsg_set_status("minishell: syntax error near unexpected token `newline'");
		if (token_head->t_type >= 2 && token_head->next && token_head->next->t_type > 1)
			errmsg_set_status("minishell: syntax error: near unexpected token redirections");
		if (token_head->t_type == 1 && token_head->next == NULL)
			errmsg_set_status("minishell: syntax error: near unexpected token `|'");
		if (token_head->t_type >= 2 && token_head->next == NULL)
			errmsg_set_status("minishell: syntax error: missing filename or delimiter after redirection");
		token_head = token_head->next;
	}
	if (g_exit_status == 2)
		return (1);
	return (0);
}

//used in parse_argv, for malloc str of strs
int	count_argv(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->t_type == 0)
	{
		count++;
		start = start->next;
	}
	return (count);
}

void	free_redirections(t_cmd *cmd_head)
{
	t_redir	*tmp;
	t_redir *redir;

	if (!cmd_head || !cmd_head->redirections)
		return ;

	redir = cmd_head->redirections;
	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_delim)
			free(redir->heredoc_delim);
		free(redir);
		redir = tmp;
	}
	cmd_head->redirections = NULL;
}

//free a linked list of t_cmd
void	free_cmd_list(t_cmd *cmd_head)
{
	t_cmd	*tmp;

	if (!cmd_head)
		return ;
	while (cmd_head)
	{
		tmp = cmd_head->next;
		// yuxins version, no need to check null before free somehow
		// ft_free_arr(cmd_head->argv);
		// free(cmd_head->quote_type);
		// free_redirections(cmd_head);
		if (cmd_head->argv)
			ft_free_arr(cmd_head->argv);
		if (cmd_head->quote_type)
			free(cmd_head->quote_type);
		if (cmd_head->redirections)
			free_redirections(cmd_head);
		if (cmd_head->cmd_path)
			free(cmd_head->cmd_path);
		free(cmd_head);
		cmd_head = tmp;
	}
}
