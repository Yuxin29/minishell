#include "minishell.h"
#include "exec.h"

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
		new->heredoc_delim = ft_strdup(file_tok->str);
		if (!new->heredoc_delim)
			return (free(new), perror("malloc: "), NULL);
		new->file = creat_heredoc_file(new->heredoc_delim);
		if (!new->file)
			return (free(new->heredoc_delim), free(new), NULL);
	}
	else
	{
		new->file = ft_strdup(file_tok->str);
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
			new_redir = create_redir_node(tokens, next);
			if (!new_redir)
				return (NULL);
			if (!cmd->redirections)
			{
				free_redirections(cmd);//added recently
				cmd->redirections = new_redir;
			}
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

//used for parse argv
int	malloc_for_agrv(t_cmd *cmd, t_token *tokens)
{
	int	len;

	len = count_argv(tokens);
	cmd->argv = malloc(sizeof(char *) * (len + 1));
	if (!cmd->argv)
	{
		perror("malloc: ");
		return (-1);
	}
	cmd->quote_type = malloc(sizeof(int) * len);
	if (!cmd->quote_type)
	{
		ft_free_arr(cmd->argv);
		perror("malloc: ");
		return (-1);
	}
	return (len);
}

// handle normal word_tokens, string and strings
// if (!tokens)
// 	return (NULL);
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
