#include "parsing.h"

// get raw_line with readline,  and the put them to t_token
t_token *get_token_list(char *raw_line)
{
    t_token     *head;
    t_token     *last;
    t_token        *new;
    int          i;
    int         len;

    head = NULL;
    last = NULL;
    new = NULL;
    i = 0;
    len = 0;
    while (raw_line[i])
    {
        while (raw_line[i] && ft_isspace(raw_line[i]))
            i++;
        if (!raw_line[i])
            return (NULL);
        new = 0;
        new = get_one_new_token(raw_line[i], len);
        if (!new)   //malloc inside
            return (free_token_list(head), NULL);
        if (!head)          //if head not initiatedm the new one is the headone, 
            head = new;
        else                //otherwise, append the new one to the end of the last one
            last->next = new;
        last = new;         //and then, change head
        i +=len;
    }
    return head;
}

//get one single new_token
t_token    *get_one_new_token(char *raw_line, int len)
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
    len = ft_strlen(new->str)
    return (new);
}

//char	*ft_substr(char const *s, unsigned int start, size_t len), malloc inside, need to null check
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