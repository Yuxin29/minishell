#include "minishell.h"

static int	check_pipe_syntax(char *line, int *i)
{
	(*i)++;
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
		return (errmsg_return_nbr(SYNTAX_ERR_PIPE, 0, 0));
	if (line[*i] == '|')
		return (errmsg_return_nbr(SYNTAX_ERR_PIPE, 0, 0));
	return (1);
}

static int	check_redir_syntax(char *line, int *i)
{
	char	c;
	int		count;

	c = line[*i];
	count = 0;
	while (line[*i] == c)
	{
		count++;
		(*i)++;
	}
	if (count > 2)
		return (errmsg_return_nbr(SYNTAX_ERR_REDIR_DOUBLE, 0, 0));
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
		return (errmsg_return_nbr(SYNTAX_ERR_REDIR_FILE_MISSING, 0, 0));
	if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
		return (errmsg_return_nbr(SYNTAX_ERR_REDIR_FILE_MISSING, 0, 0));
	return (1);
}

static int	handle_operator_syntax(char *line, int *i, int quote)
{
	if (quote)
		return (1);
	if (line[*i] == '|')
	{
		if (!check_pipe_syntax(line, i))
			return (0);
		return (2);
	}
	else if (line[*i] == '<' || line[*i] == '>')
	{
		if (!check_redir_syntax(line, i))
			return (0);
		return (2);
	}
	return (1);
}

// 1 for perror, 0 for stderr
static int	check_start_pipe_syntax(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '|')
		return (errmsg_return_nbr(SYNTAX_ERR_PIPE, 0, 0));
	return (1);
}

// syntax check on char level
//cmd->exit_status = 0;
int	check_line_syntax(char *raw_line)
{
	int		i;
	char	quote;
	char	c;
	int		ret;

	i = 0;
	quote = 0;
	if (!check_start_pipe_syntax(raw_line, &i))
		return (0);
	while (raw_line[i])
	{
		c = raw_line[i];
		if (!quote && (c == '"' || c == '\''))
			quote = c;
		else if (quote && c == quote)
			quote = 0;
		if (!quote && (c == '|' || c == '<' || c == '>'))
		{
			ret = handle_operator_syntax(raw_line, &i, quote);
			if (ret == 0)
				return (0);
			if (ret == 2)
				continue ;
		}
		i++;
	}
	return (1);
}
