#include "parsing.h"

// get raw_line with readline,  and the put them to t_token
t_token *get_token_list(char *raw_line)
{
    t_token     *head;
    t_token     *last;
    t_token        *new;
    int          i;

    head = NULL;
    last = NULL;
    i = 0;

    while (raw_line[i])
    {
        while (raw_line[i] && ft_isspace(raw_line[i]))
            i++;
        if (!raw_line[i])
            break;
        new = get_one_new_token(raw_line[i]);
        if (!new)
            return (free_token_list(head), NULL);
        if (!head)
            head = new;
        else
            last->next = new;
        last = new;
    }
    return head;
}

//char	*ft_substr(char const *s, unsigned int start, size_t len)

t_token    *get_one_new_token(char *raw_line)
{
    t_token *new;
    int     i;

    new = malloc(sizeof(t_token) * 1)
    if (!new)
        return (NULL);
    i = 0;
    if (raw_line[i] != '\'' && raw_line[i] != '"')
        token_no_quote(i, new);
    else if (raw_line[i] = '\'')
        token_single_quote(i, new);
    else if (raw_line[i] = '"')
        token_double_quote(i, new);
    if(!token->str);
        return (NULL)
    new->t_type = get_token_type(new);
    new->next = NULL;
    return (new);
}

//dealing no quote token
void    token_no_quote(int  i, t_token  *token)
{
    new->quote_type = 0;
    while (!ft_ispace(raw_line[0]))
        i++;
    new->str = ft_substr(raw_line, 0, i);
}

//dealing single quote token
void    token_single_quote(int  i, t_token  *token)
{
    new->quote_type = 1;
    while (raw_line[0] == '\'')
        i++;
    new->str = ft_substr(raw_line, 0, i);

}

//dealing double quote token
void    token_single_quote(int  i, t_token  *token)
{
    new->quote_type = 2;
       while (raw_line[0] == '"')
        i++;
    new->str = ft_substr(raw_line, 0, i);
}