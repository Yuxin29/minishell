#include "parsing.h"

// get raw_line with readline / getnextline,  and the put them to t_token
t_token *get_tokens(char *raw_line)
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

t_token    *get_one_new_token(char *raw_line)
{
    t_token *new;
    int     i;

    new = malloc(sizeof(t_token) * 1)
    if (!new)
        return (NULL);
    i = 0;
    if (raw_line[i] != SINGLE_QUOTE && raw_line[i] != DOUBLE_QUOTE)
    {
        new->quote_type = 0;
        while (!ft_ispace(raw_line[0]))
            i++;
        new->str = strncpy(raw_line, i);        //strncpy not allowed, need to change ft_strncpy
    }
    if (raw_line[i] = SINGLE_QUOTE)
    {
        new->quote_type = 1;
        i++;
        while (raw_line[0] != SINGLE_QUOTE)
            i++;
        new->str = strncpy(raw_line, i);        //strncpy not allowed, need to change ft_strncpy
    }
    if (raw_line[i] = DOUBLE_QUOTE)
    {
        new->quote_type = 2;
        i++;
        while (raw_line[0] != DOUBLE_QUOTE)
            i++;
        new->str = strncpy(raw_line, i);        //strncpy not allowed, need to change ft_strncpy
    }
    new->t_type = get_token_type(new);
    new->next = NULL;
    return (new);
    
}

t_token    *add_one_more_token(t_token *current, char *raw_line)
{


}
