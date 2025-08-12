#include "minishell.h"

// if it is a word without quotes, it can not contain special character
// with quotes, all chars are fine
int	check_special_characters(t_token *token_head)
{
	const char	*s;

	if (token_head->t_type == 0 && token_head->quote_type == 0)
	{
		s = token_head->str;
		while (*s)
		{
			if (*s == '\'' || *s == '\"')
				return (0);
			s++;
		}
		if (ft_strchr((const char *)token_head->str, ';')
			|| ft_strchr((const char *)token_head->str, '\\'))
			return (1);
	}
	return (0);
}

// syntaxprecheck before build cmd list
// can not start or end with a pipe
// after redirections it can not be followed by pipe or redirectiosn
// after pipe, it can not be followed by another pipe
// at the end of the tokens, it can not be pipe or redirections
int	check_token_syntax(t_token *token_head)
{
	if (!token_head)
		//return (1);
		return (0);
	if (token_head->t_type == 1)
		return (errmsg_return_one(SYNTAX_ERR_PIPE));
	while (token_head)
	{
		if (check_special_characters(token_head))
			return (errmsg_return_one(SYNTAX_ERR_SPECIAL_CHARS));
		if (token_head->t_type >= 1 && token_head->next && token_head->next->t_type == 1)
			return (errmsg_return_one(SYNTAX_ERR_PIPE));
		if (token_head->t_type >= 2 && token_head->next && token_head->next->t_type >= 2)
			return (errmsg_return_one(SYNTAX_ERR_REDIR_DOUBLE));
		if (token_head->t_type == 1 && token_head->next == NULL)
			return (errmsg_return_one(SYNTAX_ERR_PIPE));
		if (token_head->t_type >= 2 && token_head->next == NULL)
			return (errmsg_return_one(SYNTAX_ERR_REDIR_FILE_MISSING));
		token_head = token_head->next;
	}
	return (0);
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
// int	count_argv(t_token *start)
// {
// 	int	count;

// 	count = 0;
// 	while (start && start->t_type != 1)
// 	{
// 		if (start->t_type == 0)
// 		{
// 			count++;
// 			start = start->next;
// 		}
// 		else if (start->t_type >= 2 && start->t_type <= 5)
// 			break ;
// 		else
// 			break ;
// 	}
// 	return (count);
// }

//free the linked  redirection list within a cmd
void	free_redirections(t_cmd *cmd_head)
{
	t_redir	*tmp;
	t_redir	*redir;

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

//free a linked list of t_cmds
// yuxins version, no need to check null before free somehow, just
// ft_free_arr(cmd_head->argv);
// free(cmd_head->quote_type);
// free_redirections(cmd_head);
void	free_cmd_list(t_cmd *cmd_head)
{
	t_cmd	*tmp;

	if (!cmd_head)
		return ;
	while (cmd_head)
	{
		tmp = cmd_head->next;
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
