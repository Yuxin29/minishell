#include "parsing.h"
#include "minishell.h"

//precheck before build cmd list
// syntax check (pipes, etc.)
// 1 on errors and 0 on okeii
int check_syntax(t_token *token_head)
{
    if (!token_head)
        return (1);
    //Starts with a pipe
    if (token_head->t_type == 1)
        return (1);
    while (token_head)
    {
        // no word after ">, <, >>, << "
        // Two pipes in a row (e.g., ls || cat)
        //Pipe directly after redirection (e.g., cat < | wc)
        // Redirection followed by another redirection (e.g., cat < > file)
        //. Empty command between pipes (e.g., ls | | wc)
        token_head = token_head->next;
    }
    //Ends with a pipe
    if (token_head->t_type == 1)
        return (1);
    return (0);
}
//used in parse_argv, for malloc str of strs
int count_argv(t_token *start)
{
    int count = 0;
    while (start && start->t_type == 0)
    {
        count++;
        start = start->next;
    }
    return count;
}

//free a linked list of t_cmd
void    free_cmd_list(t_cmd *cmd_head)
{
    t_cmd *tmp;

    while(cmd_head)
    {
        tmp = cmd_head->next;
        if (cmd_head->argv)
            return ;
            //ft_free_split(cmd_head->argv);  //ft_free_split; need to enrich libft again
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
