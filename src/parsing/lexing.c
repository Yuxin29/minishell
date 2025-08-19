#include "minishell.h"

// in case of mem failure, already perrored inside it
// build the <, <<, >, >> and |
t_token	*build_operator_token(char *line, int *i, t_exec_path *cmd)
{
	t_token	*token;
	int		start;

	start = (*i)++;
	if ((line[start] == '>' && line[*i] && line[*i] == '>')
		|| (line[start] == '<' && line[*i] && line[*i] == '<'))
		(*i)++;
	token = malloc(sizeof(t_token));
	if (!token)
		return ((t_token *)free_malloc_fail_null_status(NULL, cmd));
	token->str = ft_strndup(&line[start], *i - start);
	if (!token->str)
	{
		free(token);
		return ((t_token *)free_malloc_fail_null_status(NULL, cmd));
	}
	token->quote_type = 0;
	token->next = NULL;
	get_token_type(token);
	return (token);
}

// two cases in this one: redirectons token and world token
// no need for null check. already perrored, just return null
static t_token	*build_token_from_next_word(char *line, int *i, t_exec_path *cmd)
{
	if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
		return (build_operator_token(line, i, cmd));
	else
		return (build_word_token(line, i, cmd));
}

// called in get_token list, this is the actual getting token loop
// skip all meaningless space if any, stop until '\0'
// free_token_list(head); yuxin added 0817, added same logic in all linked list
static t_token	*tokenize_loop(char *raw_line, t_exec_path *cmd)
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
		new = build_token_from_next_word(raw_line, &i, cmd);
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

// loop through raw_line and build token list.
// if passing syntax check, enter tokenize loop
// no need to check null in rawline, checked in main
// if (raw_line[0] == '\0')  // $EMPTY, not mem error, it is allowed
t_token	*get_token_list(t_exec_path *cmd, char *raw_line)
{
	cmd->exit_status = 0;
	check_raw_line_syntax(raw_line, cmd);
	if (cmd->exit_status == 2)
		return (NULL);
	precheck_special_chars_rawline(raw_line, cmd);
	if (cmd->exit_status == 2)
		return (NULL);
	return (tokenize_loop(raw_line, cmd));
}