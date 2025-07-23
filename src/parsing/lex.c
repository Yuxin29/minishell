#include "parsing.h"

// get raw_line with readline / getnextline,  and the put them to t_token
t_token    *get_tokens(char *raw_line)
{
    int i;
    t_token *start;

    start = get_one_new_token(raw_line);
    if (!start)
        return (NULL);
    i = 0;
    while (raw_line[i])
    {
        add_one_more_token(startm raw_line);
        get_one_new_token(raw_line);
    }   
    return (start);
}

t_token    *get_one_new_token(char *raw_line)
{
    t_token *new;
    int     i;

    new = malloc(sizeof(t_token) * 1)
    if (!new)
        return (NULL);
    i = 0;
    if (raw_line[0] != SINGLE_QUOTE || raw_ line[0] != DOUBLE_QUOTE)
    {
        while (!ft_ispace(raw_line[0]))
            i++;
        new->str = ft_strdup(raw_line);
        if (!new->str)
        {
            free(new);
            return(NULL);
        }
    }
    if (raw_line[0] = SINGLE_QUOTE);
        deal_single_quotes(new, raw_line);
    new->t_type = get_token_type(new);
        deal_double_quotes(new, raw_line);
    new->next = NULL;
    return (new);
    
}

t_token    *add_one_more_token(t_token *current, char *raw_line)
{


}

void        deal_single_quotes(char *raw_line)
{

}

void        deal_double_quotes(char *raw_line)
{

}


