#include "minishell.h"

// precheck validity of the expanderd raw_line
// send error msgs and set up the exit status in cmd
// !raw_line[i] check unnecessary, for example $EMPTY
void	check_raw_line_syntax(char *raw_line, t_exec_path *cmd)
{
	int	i;

	i = 0;
	while (raw_line[i])
	{
		if (raw_line[i] == '"')
		{
			i++;
			while (raw_line[i] && raw_line[i] != '"')
				i++;
		}
		else if (raw_line[i] == '\'')
		{
			i++;
			while (raw_line[i] && raw_line[i] != '\'')
				i++;
		}
		if (!raw_line[i])
		{
			errmsg_set_status(SYNTAX_ERR_QUOTES, cmd);
			return ;
		}
		i++;
	}
}

// if it is a word without quotes, it can not contain special character
// with quotes, all chars are fine
void	precheck_special_chars_rawline(char *line, t_exec_path *cmd)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double)
		{
			if (line[i] == ';' || line[i] == '\\')
			{
				errmsg_set_status(SYNTAX_ERR_SPECIAL_CHARS, cmd);
				return ;
			}
		}
		i++;
	}
}

// get the type of a token
// non mem involved in this one
void	get_token_type(t_token *token)
{
	const char	*tmp;

	tmp = token->str;
	if (token->quote_type != 0)
		token->t_type = T_WORD;
	else if (ft_strncmp(tmp, "|", 2) == 0)
		token->t_type = T_PIPE;
	else if (ft_strncmp(tmp, ">>", 3) == 0)
		token->t_type = T_APPEND;
	else if (ft_strncmp(tmp, "<<", 3) == 0)
		token->t_type = T_HEREDOC;
	else if (ft_strncmp(tmp, "<", 2) == 0)
		token->t_type = T_REDIRECT_IN;
	else if (ft_strncmp(tmp, ">", 2) == 0)
		token->t_type = T_REDIRECT_OUT;
	else
		token->t_type = T_WORD;
}

//return null if fails, null checked when it is used
char	*get_unquoted_part(char *s, int *i)
{
	int		start;
	char	*part;

	start = *i;
	while (s[*i] && !ft_isspace(s[*i]) && s[*i] != '\''
		&& s[*i] != '"' && s[*i] != '<' && s[*i] != '>' && s[*i] != '|')
		(*i)++;
	part = ft_strndup(&s[start], *i - start);
	if (!part)
		return (free_malloc_fail_null(NULL));
	return (part);
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
