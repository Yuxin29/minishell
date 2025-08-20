#include "minishell.h"

static int	check_pipe_syntax(char *line, int *i, t_exec_path *cmd)
{
	(*i)++;
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
		return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd), 0);
	if (line[*i] == '|')
		return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd), 0);
	return (1);
}

static int	check_redir_syntax(char *line, int *i, t_exec_path *cmd)
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
		return (errmsg_set_status(SYNTAX_ERR_REDIR_DOUBLE, cmd), 0);
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
		return (errmsg_set_status(SYNTAX_ERR_REDIR_FILE_MISSING, cmd), 0);
	if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
		return (errmsg_set_status(SYNTAX_ERR_REDIR_FILE_MISSING, cmd), 0);
	return (1);
}

static int	handle_operator_syntax(char *line, int *i,\
	int quote, t_exec_path *cmd)
{
	if (quote)
		return (1);
	if (line[*i] == '|')
	{
		if (!check_pipe_syntax(line, i, cmd))
			return (0);
		return (2);
	}
	else if (line[*i] == '<' || line[*i] == '>')
	{
		if (!check_redir_syntax(line, i, cmd))
			return (0);
		return (2);
	}
	return (1);
}

static int	check_start_pipe_syntax(char *line, int *i, t_exec_path *cmd)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '|')
		return (errmsg_set_status(SYNTAX_ERR_PIPE, cmd), 0);
	return (1);
}

// syntax check on char level
// //cmd->exit_status = 0; 
void	check_line_syntax(char *raw_line, t_exec_path *cmd)
{
	int		i;
	char	quote;
	char	c;

	i = 0;
	quote = 0;
	if (!check_start_pipe_syntax(raw_line, &i, cmd))
		return ;
	while (raw_line[i])
	{
		c = raw_line[i];
		if (!quote && (c == '"' || c == '\''))
			quote = c;
		else if (quote && c == quote)
			quote = 0;
		if (!quote && (c == '|' || c == '<' || c == '>'))
		{
			if (handle_operator_syntax(raw_line, &i, quote, cmd) == 0)
				return ;
			if (handle_operator_syntax(raw_line, &i, quote, cmd) == 2)
				continue ;
		}
		i++;
	}
	return ;
}
