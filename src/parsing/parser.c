#include "minishell.h"

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
		cmd_current = malloc(sizeof(t_cmd));
		if (!cmd_current)
		{
			free_cmd_list(cmd_head);
			return ((t_cmd *)free_malloc_fail_null(NULL));
		}
		ft_bzero(cmd_current, sizeof(t_cmd));
		token_head = get_one_new_cmd(token_head, cmd_current);
		if (!token_head)
		{
			if (hd_is_interrupted())
			{
				free_cmd_node(cmd_current);
				free_cmd_list(cmd_head);
				cmd->exit_status = 130;
				return (NULL);
			}
			if (!cmd_current->argv && !cmd_current->redirections)
			{
				free_cmd_node(cmd_current);
				free_cmd_list(cmd_head);
				cmd->exit_status = 0;
				return (NULL);
			}
		}
		if (!cmd_head)
			cmd_head = cmd_current;
		else
			cmd_last->next = cmd_current;
		cmd_last = cmd_current;
	}
	return (cmd_head);
}

// generate on single cmd struct
// parse_argv(cmd_current, argv_tok); this function has a return value, need to chekc
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
		if (token_head && token_head->t_type == 0)
		{
			while (token_head && token_head->t_type == 0)
				token_head = token_head->next;
		}
		else if (token_head->t_type >= 2 && token_head->t_type <= 5)
		{
			token_head = parse_redirections(cmd_current, token_head);
			if (!token_head)
				return (NULL);
		}
		if (token_head && token_head->t_type == 0)
		{
			while (token_head && token_head->t_type == 0)
				token_head = token_head->next;
		}
	}
	if (token_head && token_head->t_type == 1)
		token_head = token_head->next;
	return (token_head);
}

t_redir	*create_redir_node(t_token *redir_tok, t_token *file_tok)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ((t_redir *)free_malloc_fail_null(NULL));
	new->type = redir_tok->t_type;
	new->next = NULL;
	if (redir_tok->t_type == 5)
	{
		new->heredoc_delim = ft_strndup(file_tok->str, ft_strlen(file_tok->str));
		if (!new->heredoc_delim)
			return (free(new), perror("malloc: "), NULL);
		new->file = creat_heredoc_file(new->heredoc_delim);
		if (!new->file)
			return (free(new->heredoc_delim), free(new), NULL);
	}
	else
	{
		new->file = ft_strndup(file_tok->str, ft_strlen(file_tok->str));
		if (!new->file)
			return (free(new), (t_redir *)free_malloc_fail_null(NULL));
		new->heredoc_delim = NULL;
	}
	return (new);
}

t_token	*parse_redirections(t_cmd *cmd, t_token *tokens)
{
	t_redir	*new_redir;
	t_redir	*last;
	t_token	*next;

	while (tokens && tokens->t_type != 1)
	{
		if (tokens->t_type >= 2 && tokens->t_type <= 5)
		{
			next = tokens->next;
			if (!next || next->t_type != 0)
				return (NULL);
			new_redir = create_redir_node(tokens, next);
			if (!new_redir)
				return (NULL);
			if (!cmd->redirections)
				cmd->redirections = new_redir;
			else
			{
				last = cmd->redirections;
				while (last->next)
					last = last->next;
				last->next = new_redir;
			}
			tokens = next->next;
		}
		else
			break ;
	}
	return (tokens);
}

//handle normal word_tokens, string and strings
t_token	*parse_argv(t_cmd *cmd, t_token *tokens)
{
	int	i;
	int	len;

	if (!tokens)
		return (NULL);
	i = 0;
	len = count_argv(tokens);
	cmd->argv = malloc(sizeof(char *) * (len + 1));
	if (!cmd->argv)
		return ((t_token *)free_malloc_fail_null(NULL));
	cmd->quote_type = malloc(sizeof(int) * len);
	if (!cmd->quote_type)
	{
		ft_free_arr(cmd->argv);
		return ((t_token *)free_malloc_fail_null(NULL));
	}
	while (tokens && tokens->t_type != 1 && i < len)
	{
		if (tokens->t_type == 0)
		{
			cmd->argv[i] = ft_strndup(tokens->str, ft_strlen(tokens->str));
			if (!cmd->argv[i])
			{
				ft_free_arr(cmd->argv);
				free(cmd->quote_type);
				return ((t_token *)free_malloc_fail_null(NULL));
			}
			cmd->quote_type[i] = tokens->quote_type;
			i++;
			tokens = tokens->next;
		}
		else if (tokens->t_type >= 2 && tokens->t_type <= 5)
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
		}
		else
			break ;
	}
	cmd->argv[i] = NULL;
	return (tokens);
}
