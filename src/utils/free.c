#include "minishell.h"

//free the whole linked list
void	free_token_list(t_token *token_head)
{
	t_token	*tmp;

	if (!token_head)
		return ;
	while (token_head)
	{
		tmp = token_head->next;
		if (token_head->str)
			free(token_head->str);
		free(token_head);
		token_head = tmp;
	}
}

//free the linked redirection list within a cmd
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

// free one single cmd node
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
void	free_t_exec_path(t_exec_path *exec_cmd)
{
	if (!exec_cmd)
		return ;
	if (exec_cmd->whole_cmd)
		free_cmd_list(exec_cmd->whole_cmd);
	if (exec_cmd->envp)
		ft_free_arr(exec_cmd->envp);
	exec_cmd->whole_cmd = NULL;
	exec_cmd->envp = NULL;
}
