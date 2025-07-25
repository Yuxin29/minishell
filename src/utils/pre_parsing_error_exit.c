#include "minishell.h"

//pre_parsing_error_exit.c
void    errmsg_exit(char *msg, int status)
{
    if (msg)
        printf("%s\n", msg);
    exit(status);
}

//pre_parsing_error_exit.c
void    free_errmsg_exit(t_token *tokens, char *msg, int status)
{
    if (tokens)
        free_token_list(tokens);
    if (msg)
        printf("%s\n", msg);
    exit(status);
}

//pre_parsing_error_exit.c
void    free_tc_errmsg_exit(t_token *tokens, t_cmd *cmds, char *msg, int status)
{
    if (tokens)
        free_token_list(tokens);
    if (cmds)
        free_cmd_list(cmds);
    if (msg)
        printf("%s\n", msg);
    exit(status);
}