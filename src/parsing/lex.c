#include "parsing.h"
#include "minishell.h"

// get raw_line with readline,  and the put them to t_token
t_token *get_token_list(char *raw_line)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	int		i;
	int		len;

    head = NULL;
    last = NULL;
    new = NULL;
    i = 0;
    len = 0;
    if (check_quotes_closed(raw_line))
        return (NULL);
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
    return head;
}
//if head not initiatedm the new one is the headone,
//otherwise, append the new one to the end of the last one
//and then, change head

//get one single new_token
t_token	*get_one_new_token(char *raw_line, int *len)
{
	t_token	*new;
	int		i;

	new = malloc(sizeof(t_token) * 1); //!
	if (!new)
		return (NULL);
	i = 0;
	if (raw_line[i] != '\'' && raw_line[i] != '"')
		token_no_quote(raw_line, &i, new);
	else if (raw_line[i] == '\'')
		token_single_quote(raw_line, &i, new);
	else if (raw_line[i] == '"')
		token_double_quote(raw_line, &i, new);
	if(!new->str)
	{
		free(new);
		return (NULL);
	}
	//new->t_type = get_token_type(new);
	get_token_type(new);
	new->next = NULL;
	*len = i;
	return (new);
}

//char	*ft_substr(char const *s, unsigned int start, size_t len),
// malloc inside, need to null check
//dealing no quote token
void	token_no_quote(char *raw_line, int  *i, t_token  *token)
{
	int start;

	start = *i;
	token->quote_type = 0;
	while (raw_line[*i] && !ft_isspace(raw_line[*i]))
		(*i)++;
	token->str = ft_strndup(&raw_line[start], *i - start);
	if (!token->str) //!
		return ;
}

//dealing single quote token
void	token_single_quote(char *raw_line, int  *i, t_token  *token)
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
void	token_double_quote(char *raw_line, int  *i, t_token  *token)
{
	int start;

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
}
