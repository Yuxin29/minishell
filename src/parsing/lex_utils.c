#include "parsing.h"
#include "minishell.h"

//nothing to free in this whole files functions
//precheck validity of the raw_line
//return 1 on errors and 1 on okei
//in case of the quotes not close, 
// bash screen is blicking because it thinks that the cmd is not finished
// but we treat it as syntax error
//nothing need to be freeed here
void	check_raw_line_syntax(char *raw_line)
{
	int	i;

	i = 0;
	if (!raw_line || !raw_line[i])
		return ;
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
			errmsg_exit("minishell: syntax error: unclosed quotes", 2);
		i++;
	}
}

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

//no need to null check in ft_strjoin, it will return null if failed
//but need to null check when calling ft_strjoin_free
char	*ft_strjoin_free(char *s1, char *s2)
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

void get_quote_type(t_token *token, char q)
{
    if (q == '\'')
        token->quote_type = 1;
    else if (q == '"')
        token->quote_type = 2;
    else
        token->quote_type = 0;
}
