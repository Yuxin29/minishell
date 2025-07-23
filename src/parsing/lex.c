#include "parsing.h"
#include "libft.h"

static int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}


char	*ft_strndup(const char *s, int n)
{
	char	*dst;
	int	i;

	if (!s)
		return (NULL);
	dst = malloc(sizeof(char) * (n + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = s[i];
		i++;
	}
	dst[n] = '\0';
	return (dst);
}

// get raw_line with readline,  and the put them to t_token
t_token *get_token_list(char *raw_line)
{
    t_token     *head;
    t_token     *last;
    t_token      *new;
    int          i;
    int         len;

    head = NULL;
    last = NULL;
    new = NULL;
    i = 0;
    len = 0;
    while (raw_line[i])
    {
        while (raw_line[i] != '\0' && ft_isspace(raw_line[i]) == 0)
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
t_token    *get_one_new_token(char *raw_line, int *len)
{
    t_token *new;
    int     i;

    new = safe_malloc(sizeof(t_token) * 1);
    i = 0;
    if (raw_line[i] != '\'' && raw_line[i] != '"')
        token_no_quote(raw_line, i, new);
    else if (raw_line[i] == '\'')
        token_single_quote(raw_line, i, new);
    else if (raw_line[i] == '"')
        token_double_quote(raw_line, i, new);
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
void    token_no_quote(char *raw_line, int  i, t_token  *token)
{
    i++;
    token->quote_type = 0;
    while (!ft_isspace(raw_line[0]))
        i++;
    token->str = ft_strndup(raw_line, i);
    i++;
}

//dealing single quote token
void    token_single_quote(char *raw_line, int  i, t_token  *token)
{
    i++;
    token->quote_type = 1;
    while (raw_line[0] != '\'')
        i++;
    token->str = ft_strndup(raw_line, i);
    i++;
}

//dealing double quote token
void    token_double_quote(char *raw_line, int  i, t_token  *token)
{
    int start;
        
    i++;
    start = i;
    token->quote_type = 2;
    while (raw_line[0] != '"')
        i++;
    token->str = ft_strndup(raw_line, i);
    i++;
}
