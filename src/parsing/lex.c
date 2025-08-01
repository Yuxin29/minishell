#include "parsing.h"
#include "minishell.h"

// loop through raw_line and build token list
//no need to check null in rawline, checked in main
t_token	*get_token_list(char *raw_line)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	check_raw_line_syntax(raw_line);
	while (raw_line[i])
	{
		while (raw_line[i] && ft_isspace(raw_line[i]))
			i++;
		if (!raw_line[i])
			break ;
		new = build_token_from_next_word(raw_line, &i);
		if (!new)
			return (free_token_list(head), NULL);
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

	temp = NULL;
	while (line[*i] && !ft_isspace(line[*i])
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		q = line[*i];
		if (line[*i] == '\'' || line[*i] == '"')
			part = get_quoted_part(line, i);
		else
			part = get_unquoted_part(line, i);
		if (!part)
		{
			if (temp)
				free(temp);
			return (NULL);
		}
		temp = ft_strjoin_free(temp, part);
		if (!temp)
			return (NULL);
	}
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(temp), NULL);
	token->str = ft_strdup(temp);
	free(temp);
	if (!(token->str))
		return (free(token), NULL);
	get_quote_type(token, q);
	token->next = NULL;
	get_token_type(token);
	return (token);
}

// merge quoted/unquoted fragments into one token
t_token	*build_token_from_next_word(char *line, int *i)
{
	char	*part;
	t_token	*token;
	int		start;

	if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
	{
		start = (*i)++;
		if ((line[start] == '>' && line[*i] == '>') || (line[start] == '<' && line[*i] == '<'))
			(*i)++;
		part = ft_strndup(&line[start], *i - start);
		if (!part)
			return (NULL);
		token = malloc(sizeof(t_token));
		if (!token)
			return (free(part), NULL);
		token->str = ft_strdup(part);
		free(part);
		if (!(token->str))
			return (free(token), NULL);
		token->quote_type = 0;
		token->next = NULL;
		get_token_type(token);
		return (token);
	}
	return (build_word_token(line, i));
}

//return null if fails
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
	if (s[*i] == quote)
		(*i)++;
	return (part);
}

//return null if fails
char	*get_unquoted_part(char *s, int *i)
{
	int	start;

	start = *i;
	while (s[*i] && !ft_isspace(s[*i]) && s[*i] != '\''
		&& s[*i] != '"' && s[*i] != '<' && s[*i] != '>' && s[*i] != '|')
		(*i)++;
	return (ft_strndup(&s[start], *i - start));
}
