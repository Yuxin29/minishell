#include "parsing.h"
#include "minishell.h"

// loop through raw_line and build token list
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

// merge quoted/unquoted fragments into one token
t_token	*build_token_from_next_word(char *line, int *i)
{
	char	*temp;
	char	*part;
	t_token	*token;
    int		start;

    temp = NULL;
    if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')//take out later
	{
		start = (*i)++;
		if (line[start] == '>' && line[*i] == '>')
			(*i)++;
		part = ft_strndup(&line[start], *i - start);
		token = malloc(sizeof(t_token));
        token->str = ft_strdup(part);
		if (!token || !(token->str))
			return (free(part), free(token), NULL);
		free(part);
		token->quote_type = 0;
		token->next = NULL;
		get_token_type(token);
		return (token);
	}
	while (line[*i] && !ft_isspace(line[*i]) && line[*i] != '<' && line[*i] != '>' && line[*i] != '|') //take out later
	{
		part = NULL;
		if (line[*i] == '\'')
			part = get_quoted_part(line, i, '\'');
		else if (line[*i] == '"')
			part = get_quoted_part(line, i, '"');
		else
			part = get_unquoted_part(line, i);
		if (!part)
			return (free(temp), NULL);
		temp = ft_strjoin_free(temp, part);
	}
	token = malloc(sizeof(t_token));
    token->str = ft_strdup(temp);
	if (!token || !(token->str))
		return (free(temp), free(token), NULL);
	free(temp);
	token->quote_type = 0;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

char *get_quoted_part(char *s, int *i, char quote)
{
	int		start;
    char	*part;

    start = ++(*i);
	while (s[*i] && s[*i] != quote)
		(*i)++;
	part = ft_strndup(&s[start], *i - start);
	if (s[*i] == quote)
		(*i)++;
	return (part);
}

char *get_unquoted_part(char *s, int *i)
{
	int	start;

    start = *i;
	while (s[*i] && !ft_isspace(s[*i]) && s[*i] != '\'' 
        && s[*i] != '"' && s[*i] != '<' && s[*i] != '>' && s[*i] != '|')
		(*i)++;
	return (ft_strndup(&s[start], *i - start));
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1)
		joined = ft_strjoin("", s2);
	else
		joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

/*
// get raw_line with readline,  and the put them to t_token
t_token	*get_token_list(char *raw_line)
{
	t_token		*head;
	t_token		*last;
	t_token		*new;
	int			i;
	int			len;

	head = NULL;
	last = NULL;
	new = NULL;
	i = 0;
	len = 0;
	check_raw_line_syntax(raw_line);
	while (raw_line[i])
	{
		while (raw_line[i] != '\0' && ft_isspace(raw_line[i]) != 0)
			i++;
		if (!raw_line[i])
			break ;
		new = get_one_new_token(&raw_line[i], &len);
		if (!new)
		{
			free_token_list(head);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
		i += len;
	}
	return (head);
}

//if head not initiated, m the new one is the headone,
//otherwise, append the new one to the end of the last one
//and then, change head
//get one single new_token
t_token	*get_one_new_token(char *raw_line, int *len)
{
	t_token	*new;
	int		i;

	new = malloc(sizeof(t_token) * 1);
	if (!new)
		return (NULL);
	i = 0;
	if (raw_line[i] != '\'' && raw_line[i] != '"')
		token_no_quote(raw_line, &i, new);
	else if (raw_line[i] == '\'')
		token_single_quote(raw_line, &i, new);
	else if (raw_line[i] == '"')
		token_double_quote(raw_line, &i, new);
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	get_token_type(new);
	new->next = NULL;
	*len = i;
	return (new);
}

//char	*ft_substr(char const *s, unsigned int start, size_t len),
// malloc inside, need to null check
//dealing no quote token
void	token_no_quote(char *raw_line, int *i, t_token *token)
{
	int	start;

	start = *i;
	token->quote_type = 0;
	while (raw_line[*i] && !ft_isspace(raw_line[*i]))
		(*i)++;
	token->str = ft_strndup(&raw_line[start], *i - start);
	if (!token->str)
		return ;
}

//dealing single quote token
void	token_single_quote(char *raw_line, int *i, t_token *token)
{
	int	start;

	(*i)++;
	start = *i;
	token->quote_type = 1;
	while (raw_line[*i] && raw_line[*i] != '\'')
		(*i)++;
	token->str = ft_strndup(&raw_line[start], *i - start);
	if (!token->str)
		return ;
	if (raw_line[*i] == '\'')
		(*i)++;
}

//dealing double quote token
void	token_double_quote(char *raw_line, int *i, t_token *token)
{
	int	start;

	(*i)++;
	start = *i;
	token->quote_type = 2;
	while (raw_line[*i] && raw_line[*i] != '"')
		(*i)++;
	token->str = ft_strndup(&raw_line[start], *i - start);
	if (!token->str)
		return ;
	if (raw_line[*i] == '"')
		(*i)++;
}*/
