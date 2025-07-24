#include "parsing.h"

//to find / recognize a cmd, we need to find a pipe
//basically, before the pipe is a cmd, after it is pipe.
//if no pipe, then this is a single cmd

// build command structures
t_cmd *build_command_list(t_token *token_head)
{
    t_cmd   *cmd_head;
    t_cmd   *cmd_current;
    t_cmd   *cmd_last;
    int     i;
    int     len;

    if (check_syntax(token_head))
        return (NULL);     //first precheck if the token list is valid or have syntax errors
    cmd_head = NULL;
    cmd_current = NULL;
    cmd_last = NULL;
    while (token_head && token_head->next)
    {
        cmd_current = safe_malloc(sizeof(t_cmd));
        ft_bzero(cmd_current, sizeof(t_cmd));
        if (!cmd_head)
            cmd_head = cmd_current;
        else
            cmd_last->next = cmd_current;
        cmd_last = cmd_current;
        while (token_head->t_type != 1) 
        {    
            if (token_head->t_type == 0)
            {
                i = 0;
                while (token_head-next->t_type == 0)  //this whole package should go to get_argv helper
                {
                    //**argv needs malloc first, how do I know how many *av to mallocn, need a helper here
                    len = count_argv(token_head);
                    cmd_head->argv = safe_malloc(siof(char *) * len);
                    while (i < len)
                    {
                        cmd_head->argv[i] = ft_strndup(token_head->str, ft_strlen(token_head->str));
                        if (!cmd_head->argv) //we should write a safe strdup here
                        {
                            free(cmd_current);
                            return (NULL);
                        }
                        token_head =  token_head->next;
                        i++;
                    } 
                } 
            }
            if (token_head->t_type == 2 || token_head->t_type == 5)    //redirections, need another helpers
                parse_redirections(cmd, token_head);
            token_head =  token_head->next;
        }
        if (token_head->t_type == 1)    //hit the pipe, this should be the end of this cmd
            cmd_head->next = cmd_new;
    }
    return cmd_head
}

// handle redirections
// for redirection, I feel dfficult becaust I need to check next token to know if this token str is a infile or outfile
void parse_redirections(t_cmd *cmd, t_token *tokens)
{
    if (token_head->next->t_type == 2 && token_head->t_type == 0)
    {
        cmd_head->infile = ft_strndup(token_head->str, ft_strlen(token_head->str));
        if (!cmd_head->argv)
        {
            return (NULL);  //we should write a safe strdup here
            i++;
        }
    }
    if ((token_head->next->t_type == 3 || token_head->next->t_type == 4) && token_head->t_type == 0)
    {
        cmd_head->outfile = ft_strndup(token_head->next->str, ft_strlen(token_head->next->str));
        if (!cmd_head->argv)
        {
            return (NULL);  //we should write a safe strdup here
            i++;
        }
    }
}

// group commands by pipe
//do I need it or not???
t_cmd *group_by_pipes(t_token *tokens)
{


}
