#ifndef UTILS_H
# define UTILS_H

# include "parsing.h"
# include "minishell.h"

typedef struct s_cmd t_cmd; 
typedef struct s_token t_token; 
typedef struct s_exec_path t_exec_path; 

//error_exit_gen.c
void	error_and_return(const char *msg, t_exec_path *cmd_and_path);
void	free_t_exec_path(t_exec_path *cmd_and_path);

//pre_parsing_error_exit.c
void    errmsg_exit(char *msg, int status);
void    free_errmsg_exit(t_token *tokens, char *msg, int status);
void    free_tc_errmsg_exit(t_token *tokens, t_cmd *cmds, char *msg, int status);


//str_utils.c
void	check_strndup(char *str, t_cmd *cmd, t_token *tokens);
char	*ft_strjoin_free(char *s1, char *s2);

#endif
