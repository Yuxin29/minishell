#include "parsing.h"
#include "minishell.h"

// if it is a word without quotes, it can not contain special character
static int	check_special_characters(t_token *token_head)
{
	if (token_head->t_type == 0 && token_head->quote_type == 0)
	{
		if (ft_strchr((const char *)token_head->str, ';')
			|| ft_strchr((const char *)token_head->str, '\''))
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
void	check_token_syntax(t_token *token_head)
{
	t_token	*head;

	head = token_head;
	if (!token_head)
		return ;
	if (token_head->t_type == 1)
		free_errmsg_exit(head, "minishell: syntax error near unexpected token `|'", 2);
	while (token_head)
	{
		if (check_special_characters(token_head))
			free_errmsg_exit(head, "minishell: syntax error: special characters", 2);
		if (token_head->t_type == 1 && token_head->next && token_head->next->t_type == 1)
			free_errmsg_exit(head, "minishell: syntax error near unexpected token `|'", 2);
		if (token_head->t_type == 1 && token_head->next && token_head->next->t_type > 1)
			free_errmsg_exit(head, "minishell: syntax error near unexpected token `newline'", 2);
		if (token_head->t_type >= 2 && token_head->next && token_head->next->t_type == 1)
			free_errmsg_exit(head, "minishell: ssyntax error near unexpected token `|'", 2);
		if (token_head->t_type >= 2 && token_head->next && token_head->next->t_type > 1)
			free_errmsg_exit(head, "minishell: syntax error: near unexpected token redirections", 2);
		if (token_head->t_type == 1 && token_head->next == NULL)
			free_errmsg_exit(head, "minishell: syntax error: near unexpected token `|'", 2);
		if (token_head->t_type >= 2 && token_head->next == NULL)
			free_errmsg_exit(head, "minishell: syntax error: missing filename or delimiter after redirection", 2);
		token_head = token_head->next;
	}
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

//why is this necessary
void	check_strndup(char *str, t_cmd *cmd, t_token *tokens)
{
	if (!str)
	{
		free_token_list(tokens);
		free_cmd_list(cmd);
		error_and_return("malloc failed", NULL);//free_t_errmsg_exit
	}
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
		if (cmd_head->argv)
			ft_free_arr(cmd_head->argv);
		if (cmd_head->infile)
			free(cmd_head->infile);
		if (cmd_head->outfile)
			free(cmd_head->outfile);
		if (cmd_head->heredoc_delim)
			free(cmd_head->heredoc_delim);
		free(cmd_head);
		cmd_head = tmp;
	}
}
