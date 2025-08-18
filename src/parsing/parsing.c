#include "minishell.h"

// creat_heredoc_file should be moved to right before execution
t_redir	*create_redir_node(t_token *redir_tok, t_token *file_tok, t_exec_path *cmd)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ((t_redir *)free_malloc_fail_null(NULL));
	new->type = redir_tok->t_type;
	new->next = NULL;
	new->quoted = file_tok->quote_type;
	if (redir_tok->t_type == 5)
	{
		new->heredoc_delim = ft_strdup(file_tok->str);
		if (!new->heredoc_delim)
			return (free(new), perror("malloc"), (t_redir *)NULL);
		new->file = creat_heredoc_file(new->heredoc_delim, new->quoted, cmd);
		if (!new->file)
			return (free(new->heredoc_delim), free(new), NULL); //yuxin question is (!new->file already perrored in creat_heredoc_file)
	}
	else
	{
		new->file = ft_strdup(file_tok->str);
		if (!new->file)
			return (free(new), perror("malloc"), (t_redir *)NULL);
		new->heredoc_delim = NULL;
	}
	return (new);
}

// Helper: create and append redirection node
t_token	*get_one_redirection(t_cmd *cmd, t_token *tokens, t_exec_path *exec_cmd)
{
	t_redir	*new_redir;
	t_redir	*last;
	t_token	*next;

	next = tokens->next;
	new_redir = create_redir_node(tokens, next, exec_cmd);
	if (!new_redir)
		return (NULL);
	new_redir->quoted = tokens->quote_type;
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		last = cmd->redirections;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	return (next->next);
}

//free_redirections(cmd);//added recently
t_token	*parse_redirections(t_cmd *cmd, t_token *tokens, t_exec_path *exec_cmd)
{
	while (tokens && tokens->t_type != 1)
	{
		if (tokens->t_type >= 2 && tokens->t_type <= 5)
		{
			tokens = get_one_redirection(cmd, tokens, exec_cmd);
			if (!tokens)
				return (NULL);
		}
		else
			break ;
	}
	return (tokens);
}

//used for parse argv
int	malloc_for_agrv(t_cmd *cmd, t_token *tokens)
{
	int	len;

	len = count_argv(tokens);
	cmd->argv = malloc(sizeof(char *) * (len + 1));
	if (!cmd->argv)
	{
		perror("malloc");
		return (-1);
	}
	cmd->quote_type = malloc(sizeof(int) * len);
	if (!cmd->quote_type)
	{
		ft_free_arr(cmd->argv);
		perror("malloc");
		return (-1);
	}
	return (len);
}

// handle normal word_tokens, string and strings
// tokens = tokens->next->next; safe becuase syntax already checked
t_token	*parse_argv(t_cmd *cmd, t_token *tokens)
{
	int	i;

	i = 0;
	if (malloc_for_agrv(cmd, tokens) == -1)
		return (NULL);
	while (tokens && tokens->t_type != 1)
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
			tokens = tokens->next->next;
	}
	cmd->argv[i] = NULL;
	return (tokens);
}
