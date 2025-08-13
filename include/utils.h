#ifndef UTILS_H
# define UTILS_H

typedef struct s_cmd t_cmd;
typedef struct s_token t_token;
typedef struct s_exec_path t_exec_path;

//free_error_exit.c
void	errmsg_set_status(char *msg, t_exec_path *cmd);
char		*free_malloc_fail_null(char	*str);//preparsing
void	    free_cmd_node(t_cmd *c);
void		free_t_exec_path(t_exec_path *cmd_and_path);//post_parsing

//str_utils.c
//needed for expander
char		*ft_strjoin_free(char *s1, char *s2);
//needed for builtin
long long	ft_atoll(char *str);
int			ft_is_numeric(char *str);

#endif
