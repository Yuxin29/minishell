#include "parsing.h"
#include "minishell.h"

//to find / recognize a cmd, we need to find a pipe
//basically, before the pipe is a cmd, after it is pipe.
//if no pipe, then this is a single cmd

// build command structures
t_cmd *build_command_list(t_token *token_head)
{
    t_cmd   *cmd_head;
    t_cmd   *cmd_current;
    t_cmd   *cmd_last;

    //if (check_syntax(token_head))
    //    return (NULL);     //first precheck if the token list is valid or have syntax errors
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
        while (token_head && token_head->next && token_head->t_type != 1) 
        {    
            printf("so far so good 1\n");
            if (token_head->t_type == 0)
            {
                token_head = parse_argv(cmd_current, token_head);
                printf("so far so good 2\n");
            }
            else if (token_head->t_type >= 2)    //redirections, need another helpers
                token_head = parse_redirections(cmd_current, token_head);
        }
        if (token_head && token_head->t_type == 1)    //hit the pipe, this should be the end of this cmd
            token_head = token_head->next;
        printf("so far so good 3\n");
    }
    printf("so far so good 4\n");
    return (cmd_head);
}

// handle redirections
// for redirection, I feel dfficult becaust I need to check next token to know if this token str is a infile or outfile
t_token *parse_redirections(t_cmd *cmd, t_token *tokens)
{
    t_token *next;

    if (!tokens || !tokens->next)
        return NULL;
    next = tokens->next;
    if (tokens->t_type == 2)
        cmd->infile = ft_strndup(next->str, ft_strlen(next->str));
    if (tokens->t_type == 3)
    {
        cmd->outfile = ft_strndup(next->str, ft_strlen(next->str));
        cmd->append_out = 0;
    }
    if (tokens->t_type == 4)
    {
        cmd->outfile = ft_strndup(next->str, ft_strlen(next->str));
        cmd->append_out = 1;
    }
    if (tokens->t_type == 5)
        cmd->heredoc_delim = ft_strndup(next->str, ft_strlen(next->str));
    //all the ft_strndup needs to be null checked, should we do a safe string dup
    return (next->next);
}
//handle normal word_tokens, string and strings
t_token *parse_argv(t_cmd *cmd, t_token *tokens)
{
    int i;
    int len;
    
    if (!tokens)
        return NULL;
    //if (!cmd->argv)
        //return NULL;
    i = 0;
    len = count_argv(tokens);
    cmd->argv = safe_malloc(sizeof(char *) * (len + 1));
    while (i < len && tokens && tokens->t_type == 0)
    {   
        cmd->argv[i] = ft_strndup(tokens->str, ft_strlen(tokens->str));
        if (!cmd->argv[i])
        {
            //free(cmd);
            return (NULL);
        }
        tokens = tokens->next;
        i++;
    } 
    cmd->argv[i] = NULL;
    return (tokens);
}
