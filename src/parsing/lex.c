#include "parsing.h"
#include "minishell.h"

// loop through raw_line and build token list
//no need to check null in rawline, checked in main
t_token	*get_token_list(t_exec_path *cmd, char *raw_line)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	if (check_raw_line_syntax(raw_line) == 1)
	{
		cmd->exit_status = 2;
		return (NULL);
	}
	while (raw_line[i])
	{
		while (raw_line[i] && ft_isspace(raw_line[i]))
			i++;
		if (!raw_line[i])
			break ;
		new = build_token_from_next_word(raw_line, &i);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
	}
	return (head);
}

// if it is a world token
t_token	*build_word_token(char *line, int *i)
{
	t_token	*token;
	char	*temp;
	char	*part;
	char	q;
	char	part_quote;

	q = 0;
	temp = NULL;
	while (line[*i] && !ft_isspace(line[*i]) && line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		if (line[*i] == '$' && line[*i + 1] == '"')
		{
			(*i)++; // consume '$'
			part = get_quoted_part(line, i);
			part_quote = 3; // special $" quote type
		}
		else if (line[*i] == '\'')
		{
			part = get_quoted_part(line, i);
			part_quote = 1;
		}
		else if (line[*i] == '"')
		{
			part = get_quoted_part(line, i);
			part_quote = 2;
		}
		else
		{
			part = get_unquoted_part(line, i);
			part_quote = 0;
		}
		if (!part)
			return (NULL);
		if (q == 0)
			q = part_quote;
		else if (q != part_quote)
			q = 0; // mixed quotes => no quote
		temp = ft_strjoin_free(temp, part);
		if (!temp)
			return ((t_token *)free_malloc_fail_null(NULL));
	}
	token = malloc(sizeof(t_token));
	if (!token)
		return ((t_token *)free_malloc_fail_null(temp));
	token->str = temp;
	token->quote_type = q;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

//in case of mem failure, already perrored inside it
t_token	*build_operator_token(char *line, int *i)
{
	t_token	*token;
	int		start;

	start = (*i)++;
	if ((line[start] == '>' && line[*i] && line[*i] == '>')
		|| (line[start] == '<' && line[*i] && line[*i] == '<'))
		(*i)++;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strndup(&line[start], *i - start);
	if (!token->str)
	{
		free(token);
		return ((t_token *)free_malloc_fail_null(NULL));
	}
	token->quote_type = 0;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

//two cases in this one: redirectons token and world token
//no need for null check. already perrored, just return null
t_token	*build_token_from_next_word(char *line, int *i)
{
	if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
		return (build_operator_token(line, i));
	return (build_word_token(line, i));
}

//return null if fails, null checked when it is used
char	*get_quoted_part(char *s, int *i)
{
	int		start;
	char	*part;
	char	quote;

	if (s[*i] == '\'')
		quote = '\'';
	else
		quote = '"';
	start = ++(*i);
	while (s[*i] && s[*i] != quote)
		(*i)++;
	part = ft_strndup(&s[start], *i - start);
	if (!part)
		return (free_malloc_fail_null(NULL));
	if (s[*i] == quote)
		(*i)++;
	return (part);
}
