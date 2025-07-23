#include "parsing.h"

//to find / recognize a cmd, we need to find a pipe
//basically, before the pipe is a cmd, after it is pipe.
//if no pipe, then this is a single cmd

// build command structures
t_cmd *build_command_list(t_token *tokens);
{
    t_cmd   *cmd_head;
    t_cmd   *cmd_new;
    int     i;

    if (check_syntax)
        return (NULL);     //first precheck if the token list is valid or have syntax errors
    cmd_head = safe_malloc(sizeof(t_cmd));
    cmd_new = safe_malloc(sizeof(t_cmd));
    while (token_head)
    {
        i = 0;
        if (token_head->t_type == 0)
        {
            cmd_head->argv[i] = ft_strndup(token_head->str, ft_strlen(token_head->str));
            if (!cmd_head->argv)
                return (NULL);  //we should write a safe strdup here
            i++;
        }
        if (token_head->t_type >= 2 &&  token_head->t_type <= 2)    //redirections, need another helpers
            parse_redirections(t_cmd *cmd, t_token **tokens)
        if (token_head->t_type == 1)    //hit the pipe, this should be the end of this cmd
            cmd_head->next = cmd_new;
        token_head =  token_head->next;  //go though the whole list
    }
}

// handle redirections
// for redirection, I feel dfficult becaust I need to check next token to know if this token str is a infile or outfile
void parse_redirections(t_cmd *cmd, t_token **tokens)
{


}

// group commands by pipe
t_cmd *group_by_pipes(t_token *tokens)
{


}
