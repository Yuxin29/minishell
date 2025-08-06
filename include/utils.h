#ifndef UTILS_H
# define UTILS_H

# include "parsing.h"
# include "minishell.h"


typedef struct s_cmd t_cmd;
typedef struct s_token t_token;
typedef struct s_exec_path t_exec_path;

//post_parsing_error_exit.c
void	error_and_return(const char *msg, t_exec_path *cmd_and_path);
void	free_t_exec_path(t_exec_path *cmd_and_path);

//pre_parsing_error_exit.c
void	errmsg_set_status(char *msg);
char	*free_malloc_fail_null(char	*str);

//str_utils.c
char	*check_strdup(char *dup, char *original);
char	*ft_strjoin_free(char *s1, char *s2);

#endif
