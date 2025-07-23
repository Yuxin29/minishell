#include "parsing.h"

void	get_token_type(t_token *token)
{
    if (token->str = '|')
        token->t_type = T_PIPE;
    else if (token->str = '<')
        token->t_type = T_REDIRECT_IN;
    else if (token->str = '>')
        token->t_type = T_REDIRECT_OUT;
    else if (token->str = '<<')
        token->t_type = T_APPEND;
    else if (token->str = '>>')
        token->t_type = T_HEREDOC;
    else (token->str = '|')
        token->t_type = T_WORD;
}

//free the whole linked list
void    free_token_list(t_token *token)
{


}