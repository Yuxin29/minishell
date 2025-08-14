#include "minishell.h"

static t_token	*tokenize_loop(char *raw_line)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (raw_line[i])
	{
		while (ft_isspace(raw_line[i]))
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

// loop through raw_line and build token list
//no need to check null in rawline, checked in main
//	if (raw_line[0] == '\0')  // $EMPTY,  //not mem error
t_token	*get_token_list(t_exec_path *cmd, char *raw_line)
{
	check_raw_line_syntax(raw_line, cmd);
	if (cmd->exit_status == 2)
		return (NULL);
	return (tokenize_loop(raw_line));
}

char	*get_part(char *line, int *i, char	*part_quote)
{
	char	*part;

	if (line[*i] == '$' && line[*i + 1] == '"')
	{
		(*i)++;
		part = get_quoted_part(line, i);
		*part_quote = 3;
	}
	else if (line[*i] == '\'')
	{
		part = get_quoted_part(line, i);
		*part_quote = 1;
	}
	else if (line[*i] == '"')
	{
		part = get_quoted_part(line, i);
		*part_quote = 2;
	}
	else
	{
		part = get_unquoted_part(line, i);
		*part_quote = 0;
	}
	return (part);
}

t_token	*malloc_and_set_token(char *temp, int q)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return ((t_token *)free_malloc_fail_null(temp));
	token->str = temp;
	token->quote_type = q;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

static char	*append_next_part(char *temp, char *line, int *i, char *part_quote)
{
	char	*part;

	part = get_part(line, i, part_quote);
	if (!part)
	{
		if (temp)
			free(temp);
		return (free_malloc_fail_null(NULL));
	}
	temp = ft_strjoin_free(temp, part);
	if (!temp)
		return (free_malloc_fail_null(NULL));
	return (temp);
}

// if it is a world token
t_token	*build_word_token(char *line, int *i)
{
	char	*temp;
	char	q;
	char	part_quote;

	q = 0;
	temp = NULL;
	while (line[*i] && !ft_isspace(line[*i])
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		temp = append_next_part(temp, line, i, &part_quote);
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
	return (malloc_and_set_token(temp, q));
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
		return ((t_token *)free_malloc_fail_null(NULL));
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
	else
		return (build_word_token(line, i));
}
