#ifndef UTILS_H
# define UTILS_H

//error_exit_gen.c
void	*error_and_return(const char *msg);
void	*safe_malloc(size_t size);

//pre_parsing_error_exit.c
int    errmsg_exit(char *msg, int status);
int    free_errmsg_exit(t_token *tokens, char *msg, int status);
int    free_tc_errmsg_exit(t_token *tokens, t_cmd *cmds, char *msg, int status);

#endif
