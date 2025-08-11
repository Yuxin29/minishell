#include "minishell.h"

//precheck validity of the raw_line
//return 1 on errors and 1 on okei
//in case of the quotes not close, bash waits for the second quotes
// but we treat it as syntax error
// nothing need to be freed here
int	check_raw_line_syntax(char *raw_line)
{
	int	i;

	i = 0;
	if (!raw_line || !raw_line[i])
		return (1);
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
			return (errmsg_return_one(SYNTAX_ERR_QUOTES));
		i++;
	}
	return (0);
}

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

//free the whole linked list
void	free_token_list(t_token *token_head)
{
	t_token	*tmp;

	if (!token_head)
		return ;
	while (token_head)
	{
		tmp = token_head->next;
		if (token_head->str)
			free(token_head->str);
		free(token_head);
		token_head = tmp;
	}
}

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
