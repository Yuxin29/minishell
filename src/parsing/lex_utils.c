#include "minishell.h"

//precheck validity of the expanderd raw_line
//return 1 on errors and 1 on okei
//in case of the quotes not close, we treat it as syntax error
// nothing need to be freed here
// !raw_line[i] check unnecessary, for example $EMPTY
void	check_raw_line_syntax(char *raw_line, t_exec_path *cmd)
{
	int	i;

	i = 0;
	cmd->exit_status = 0;
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
			cmd->exit_status = 2;
			ft_putendl_fd(SYNTAX_ERR_QUOTES, 2);
		}
		i++;
	}
}

// get the type of a token
//non mem involved in this one
void	get_token_type(t_token *token)
{
	const char	*tmp;

	tmp = token->str;
	if (token->quote_type != 0)
		token->t_type = T_WORD;
	else if (ft_strncmp(tmp, "|", 2) == 0)
		token->t_type = T_PIPE;
	else if (ft_strncmp(tmp, "<", 2) == 0)
		token->t_type = T_REDIRECT_IN;
	else if (ft_strncmp(tmp, ">", 2) == 0)
		token->t_type = T_REDIRECT_OUT;
	else if (ft_strncmp(tmp, ">>", 3) == 0)
		token->t_type = T_APPEND;
	else if (ft_strncmp(tmp, "<<", 3) == 0)
		token->t_type = T_HEREDOC;
	else
		token->t_type = T_WORD;
}

//get the quote type if a single token
// 0: no quote; 1: single quote; 2: double quote, 3: QUOTE_DOLLAR_DOUBLE
//non mem involved in this one
void	get_quote_type(t_token *token, char q)
{
	if (q == '\'')
		token->quote_type = 1;
	else if (q == '"')
		token->quote_type = 2;
	else
		token->quote_type = 0;
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
