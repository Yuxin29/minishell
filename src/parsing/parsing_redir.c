#include "minishell.h"

static int	has_multiple_words(const char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	while (*s && *s != ' ' && *s != '\t')
		s++;
	while (*s == ' ' || *s == '\t')
		s++;
	if (*s)
		return (1);
	return (0);
}

static int	handle_normal_redir(t_redir *new,\
	t_token *file_tok, t_exec_path *cmd)
{
	char	*new_line;

	new->heredoc_delim = NULL;
	if (ft_strchr(file_tok->str, '$'))
	{
		new_line = pre_expand_line(cmd, file_tok->str);
		if (!new_line)
			return (errmsg_return_nbr("malloc", 1, 0));
		if (!new_line[0] || has_multiple_words(new_line))
		{
			new->file = NULL;
			new->is_ambiguous = 1;
			free(new_line);
			return (1);
		}
	}
	else
	{
		new_line = ft_strdup(file_tok->str);
		if (!new_line)
			return (errmsg_return_nbr("malloc", 1, 0));
	}
	new->file = new_line;
	return (1);
}

// creat_heredoc_file should be moved to right before execution
// (!new->file already perrored in creat_heredoc_file)
static t_redir	*create_redir_node(t_token *redir_tok, t_token *file_tok,
	t_exec_path *cmd)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ((t_redir *)free_malloc_fail_null(NULL));
	new->type = redir_tok->t_type;
	new->next = NULL;
	new->quoted = file_tok->quote_type;
	new->is_ambiguous = 0;
	if (redir_tok->t_type == 5)
	{
		new->heredoc_delim = ft_strdup(file_tok->str);
		if (!new->heredoc_delim)
			return (free(new), perror("malloc"), NULL);
		new->file = creat_heredoc_file(new->heredoc_delim, new->quoted, cmd);
		if (!new->file)
			return (free(new->heredoc_delim), free(new), NULL);
	}
	else
	{
		if (!handle_normal_redir(new, file_tok, cmd))
			return (free(new), NULL);
	}
	return (new);
}

// Helper: create and append redirection node
static t_token	*get_one_redir(t_cmd *cmd,\
	t_token *tokens, t_exec_path *exec_cmd)
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
			tokens = get_one_redir(cmd, tokens, exec_cmd);
			if (!tokens)
				return (NULL);
		}
		else
			break ;
	}
	return (tokens);
}
