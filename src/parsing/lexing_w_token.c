#include "minishell.h"

char	*get_part(char *line, int *i, char	*part_quote, t_exec_path *cmd)
{
	char	*part;

	if (line[*i] == '$' && line[*i + 1] == '"')
	{
		(*i)++;
		part = get_quoted_part(line, i, cmd);
		*part_quote = 3;
	}
	else if (line[*i] == '\'')
	{
		part = get_quoted_part(line, i, cmd);
		*part_quote = 1;
	}
	else if (line[*i] == '"')
	{
		part = get_quoted_part(line, i, cmd);
		*part_quote = 2;
	}
	else
	{
		part = get_unquoted_part(line, i, cmd);
		*part_quote = 0;
	}
	return (part);
}

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

char	*append_next_part(char *temp, char *line, int *i, char *part_quote, t_exec_path *cmd)
{
	char	*part;

	part = get_part(line, i, part_quote, cmd);
	if (!part)
	{
		if (temp)
			free(temp);
		return (NULL);
	}
	temp = ft_strjoin_free(temp, part);
	if (!temp)
		return (free_malloc_fail_null_status(part, cmd));
	return (temp);
}

// if it is a world token
t_token	*build_word_token(char *line, int *i, t_exec_path *cmd)
{
	char	*temp;
	char	q;
	char	part_quote;

	q = 0;
	temp = NULL;
	while (line[*i] && !ft_isspace(line[*i])
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		temp = append_next_part(temp, line, i, &part_quote, cmd);
		if (!temp)
			return (NULL);
		if (q == 0)
			q = part_quote;
		else if (q != part_quote)
			q = 0;
		if (part_quote && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'
				|| ft_isspace(line[*i]) || line[*i] == '\0'))
			break ;
	}
	return (malloc_and_set_token(temp, q, cmd));
}
