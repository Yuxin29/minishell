#ifndef UTILS_H
# define UTILS_H

//error_exit_gen.c
void	*error_and_return(const char *msg);
void	*safe_malloc(size_t size);

//pre_parsing_error_exit.c
void    errmsg_exit(char *msg, int status);
void    free_errmsg_exit(t_token *tokens, char *msg, int status);
void    free_tc_errmsg_exit(t_token *tokens, t_cmd *cmds, char *msg, int status);

#endif
