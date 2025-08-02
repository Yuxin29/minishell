
//to find / recognize a cmd, we need to find a pipe
//basically, before the pipe is a cmd, after it is pipe.
//if no pipe, then this is a single cmd
//no need to null check token head, if null, exited earlier
t_cmd	*build_command_list(t_token *token_head)
{
	t_cmd	*cmd_head;
	t_cmd	*cmd_current;
	t_cmd	*cmd_last;

	if (check_token_syntax(token_head) == 1)
		return (NULL);
	cmd_head = NULL;
	cmd_current = NULL;
	cmd_last = NULL;
	while (token_head)
	{
		cmd_current = malloc(sizeof(t_cmd));
		if (!cmd_current)
			return ((t_cmd *)free_malloc_fail_null(NULL));
		ft_bzero(cmd_current, sizeof(t_cmd));
		if (!cmd_head)
			cmd_head = cmd_current;
		else
			cmd_last->next = cmd_current;
		cmd_last = cmd_current;
		token_head = get_one_new_cmd(token_head, cmd_current);
		if (!token_head)
			break ;
	}
	return (cmd_head);
}

//generate on single cmd struct
t_token	*get_one_new_cmd(t_token *token_head, t_cmd *cmd_current)
{
	while (token_head && token_head->t_type != 1)
	{
		if (token_head->t_type == 0)
			token_head = parse_argv(cmd_current, token_head);
		else if (token_head->t_type >= 2 && token_head->t_type <= 5)
			token_head = parse_redirections(cmd_current, token_head);
	}
	if (token_head && token_head->t_type == 1)
		token_head = token_head->next;
	return (token_head);
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
			new_redir = malloc(sizeof(t_redir));
			if (!new_redir)
				return ((t_token *)free_malloc_fail_null(NULL));
			new_redir->file = ft_strndup(next->str, ft_strlen(next->str));
			if (!new_redir->file)
				return ((t_token *)free_malloc_fail_null(NULL));
			new_redir->type = tokens->t_type;
			new_redir->next = NULL;
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
			break;
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
	while (i < len && tokens && tokens->t_type == 0)
	{
		cmd->argv[i] = ft_strndup(tokens->str, ft_strlen(tokens->str));
		if (!cmd->argv[i])
			return ((t_token *)free_malloc_fail_null(NULL));
		tokens = tokens->next;
		i++;
	}
	cmd->argv[i] = NULL;
	return (tokens);
}
