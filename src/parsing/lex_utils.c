#include "parsing.h"
#include "minishell.h"

//precheck validity of the raw_line
//return 1 on errors and 1 on okei
//in case of the quotes not close, the screen is blicking because it thinks that the cmd is not finished
int check_quotes_closed(char *raw_line)
{
    int i;
    
    i = 0;
    if (!raw_line || !raw_line[i])
        return (1);
    //other errors, like quotes not closed
    while (ft_isspace(raw_line[i]))     //handle line with ouly space and also skip all space if there are solid chars
        i++;
    if (!raw_line || !raw_line[i])
        return (1);
    while (raw_line[i])
    {
        if (raw_line[i] == '"')
        {
            i++;
            while (raw_line[i] && raw_line[i] != '"')
                i++;
            if (!raw_line[i]) // closing quote not found
                return (1);
        }
        else if (raw_line[i] == '\'')
        {
            i++;
            while (raw_line[i] && raw_line[i] != '\'')
                i++;
            if (!raw_line[i]) // closing quote not found
                return (1);
        }
        i++;
    }
    return (0);
}

void	get_token_type(t_token *token)
{
    const char *tmp;

    tmp = token->str;
    if (ft_strncmp(tmp, "|", 2) == 0)
        token->t_type = T_PIPE;
    else if (ft_strncmp(tmp, "<", 2) == 0)
        token->t_type = T_REDIRECT_IN;
    else if (ft_strncmp(tmp, ">", 2) == 0)
        token->t_type = T_REDIRECT_OUT;
    else if (ft_strncmp(tmp, "<<", 3) == 0)
        token->t_type = T_APPEND;
    else if (ft_strncmp(tmp, ">>", 3) == 0)
        token->t_type = T_HEREDOC;
    else
        token->t_type = T_WORD;
}

//free the whole linked list
void    free_token_list(t_token *token_head)
{
    t_token *tmp;

    while(token_head)
    {
        tmp = token_head->next;
        if (token_head->str)
            free(token_head->str);
        free(token_head);
        token_head = tmp;
    }
}
