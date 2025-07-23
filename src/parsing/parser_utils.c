#include "parsing.h"

//precheck before build cmd list
// syntax check (pipes, etc.)
// 1 on errors and 0 on okeii
int check_syntax(t_token *token_head)
{
    if (!token_head)
        return (1);
    if (token_head->t_type = 1)  //it can not start with a pipe
        return (1);
    /*
    and other syntax error cases: like end with pipe, direction wrong
    for example, redirection errors: there is a >, but no outfile
    while (token)
    {
        if (...)
            ,,,
        token = token->next
    }*/
    return (0);
}

//free a linked list of t_cmd
void    free_cmd_list(t_cmd *cmd_head);
{
    t_cmd *tmp;

    while(cmd_head)
    {
        tmp = cmd_head->next;
        if (cmd_head->argv)
            ft_free_split(cmd_head->argv);  //ft_free_split; need to enrich libft again
        if (cmd_head->infile)
            free(cmd_head->infile);
        if (cmd_head->outfile)
            free(cmd_head->outfile);
        if (cmd_head->heredoc_delim)
            free(cmd_head->heredoc_delim);       
        free(cmd_head);
        cmd_head = tmp;
    }
}
