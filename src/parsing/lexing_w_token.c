#include "minishell.h"

char *get_part(char *line, int *i, char *part_quote, t_exec_path *cmd)
{
	char	*part;

	if (line[*i] == '$' && line[*i + 1] == '"')  // $"
	{
		(*i)++;
		*part_quote = 3;
		part = get_quoted_part(line, i, cmd);
	}
	else if (line[*i] == '\'')  // single quote
	{
		*part_quote = 1;
		part = get_quoted_part(line, i, cmd);
	}
	else if (line[*i] == '"')  // double quote
	{
		*part_quote = 2;
		part = get_quoted_part(line, i, cmd);
	}
	else  // unquoted
	{
		*part_quote = 0;
		part = get_unquoted_part(line, i, cmd);
	}
	return (part);
}

// after get the content of the token, I malloc and assing the content to the token->str
t_token	*malloc_and_set_token(char *temp, int q, t_exec_path *cmd)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return ((t_token *)free_malloc_fail_null_status(temp, cmd));
	token->str = temp;
	token->quote_type = q;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

// helper to store/retrieve last quote type
int	save_last_quote(int new_val, int mode)
{
	static int	last;

	last = 0;
	if (mode == 1) // set
		last = new_val;
	return (last);
}

// get the content of the world toekn
char	*append_next_part(char *temp, char *part, int part_quote, int *q)
{
	temp = ft_strjoin_free(temp, part);
	if (!temp)
		return (NULL);
	if (part_quote > *q)
		*q = part_quote;
	return (temp);
}

// build t_world token and also redirect file token
t_token	*build_word_token(char *line, int *i, t_exec_path *cmd)
{
	char	*temp;
	char	*part;
	char	part_quote;
	int		q;

	q = 0;
	temp = NULL;
	while (line[*i] && !ft_isspace(line[*i])
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		part = get_part(line, i, &part_quote, cmd);
		if (!part)
		{
			if (temp)
				free(temp);
			return (NULL);
		}
		temp = append_next_part(temp, part, part_quote, &q);
		if (!temp)
			return ((t_token *)free_malloc_fail_null_status(NULL, cmd));
		if (q && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'
				|| ft_isspace(line[*i]) || line[*i] == '\0'))
			break ;
	}
	return (malloc_and_set_token(temp, q, cmd));
}
